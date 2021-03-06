/**
 * @file
 * 
 * Daemonizes (puts it in background) the given function
 *
 * @date 2017-08-20
 * @author Patxi. Lazur
 */

#ifndef H_da51e5a8_8577_11e7_8d9f_a7fc6c39368c
#define H_da51e5a8_8577_11e7_8d9f_a7fc6c39368c
#include "safesave.hpp"
#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <iostream>
#include <ctime>
#include <cstring>
#include <syslog.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

namespace patxitron { namespace util {

typedef enum {
     DAEMON_STARTED
    ,DAEMON_ALREADY_RUNNING
    ,DAEMON_ERROR
} daemonize_result;

/**
 * Makes a daemon running the provided function and returns to the parent process.
 *
 * It follows the traditional convention of doing two forks and detach the forked
 * function from controlling terminal.
 * 
 * Note that if an empty daemon_name is provided, no persistent new_pid file is
 * created and no cheking of uniqueness is performed.
 *
 * @param daemon_name Name of the daemon. If it is not empty, it creates a file
 *                    /var/run/<daemon_name>.new_pid and checks that no other daemon
 *                    of the same name is already running.
 * @param forked_func Function to be called once daemonized.
 * @param args        Arguments to pass to the forked_func.
 *
 * @return status of the daemon.
 */
template<typename FUNC, typename... ARGS>
daemonize_result daemonize(::std::string const& daemon_name
                          ,FUNC&& forked_func, ARGS&&... args)
{
    pid_t own_pid = getpid();
    ::std::string pid_file_name;
    if (!daemon_name.empty()) {
        // Check if there is another instance running and save our pid to a file
        pid_file_name += "/var/run/";
        pid_file_name += daemon_name;
        pid_file_name += ".pid";
        {
            ::std::ifstream pidfile(pid_file_name.c_str());
            if (pidfile.is_open()) {
                pid_t other_pid = -1;
                pidfile >> other_pid;
                if (other_pid > 1 && kill(other_pid, 0) == 0) {
                    syslog(LOG_NOTICE | LOG_USER, "An instance is already running");
                    return DAEMON_ALREADY_RUNNING;
                }
            }
        }
    }
    pid_file_name = safesave(own_pid, pid_file_name);
    if (pid_file_name.empty()) {
        syslog(LOG_ERR | LOG_USER, "Cannot write pid");
        return DAEMON_ERROR;
    }

    if (pid_t new_pid = fork()) {
        if (new_pid > 0) {
            // Loop for at most 1 second waiting for daemon to start
            size_t counter = 100;
            while (counter > 0) {
                ::std::ifstream pidfile(pid_file_name.c_str());
                if (pidfile.is_open()) {
                    pid_t other_pid = own_pid;
                    pidfile >> other_pid;
                    if (other_pid > 0 && other_pid != own_pid
                                      && kill(other_pid, 0) == 0) {
                        if (daemon_name.empty()) {
                            unlink(pid_file_name.c_str());
                        }
                        return DAEMON_STARTED;
                    }
                }
                counter -= 1;
                usleep(10000);
            }
            if (daemon_name.empty()) {
                unlink(pid_file_name.c_str());
            }
            return DAEMON_ERROR;
        } else {
            syslog(LOG_ERR | LOG_USER, "First fork failed: %m");
            return DAEMON_ERROR;
        }
    }

    // Make the process a new session leader. This detaches it from the
    // terminal.
    setsid();

    // A process inherits its working directory from its parent. This could be
    // on a mounted filesystem, which means that the running daemon would
    // prevent this filesystem from being unmounted. Changing to the root
    // directory avoids this problem.
    chdir("/");

    // The file mode creation mask is also inherited from the parent process.
    // We don't want to restrict the permissions on files created by the daemon,
    // so the mask is cleared.
    umask(0);

    // A second fork ensures the process cannot acquire a controlling terminal.
    if (pid_t new_pid = fork()) {
        if (new_pid > 0) {
            exit(0);
        } else {
            syslog(LOG_ERR | LOG_USER, "Second fork failed: %m");
            exit(1);
        }
    }
    if (!daemon_name.empty()) {
        openlog(daemon_name.c_str(), LOG_PID, LOG_DAEMON);
    }

    // Close the standard streams. This decouples the daemon from the terminal
    // that started it.
    close(0);
    close(1);
    close(2);

    // We don't want the daemon to have any standard input.
    if (open("/dev/null", O_RDONLY) != 0) {
        syslog(LOG_ERR | LOG_DAEMON, "Unable to open /dev/null as stdin: %m");
        exit(1);
    }

    // Send standard output to null.
    if (open("/dev/null"
            ,O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) != 1) {
        syslog(LOG_ERR | LOG_DAEMON, "Unable to open /dev/null as stdout: %m");
        exit(1);
    }

    // Also send standard error to null.
    if (dup(1) != 2) {
        syslog(LOG_ERR | LOG_DAEMON, "Unable to dup stdout to stderr: %m");
        exit(1);
    }

    // write our new_pid to file
    own_pid = getpid();
    pid_file_name = safesave(own_pid, pid_file_name);
    if (pid_file_name.empty()) {
        syslog(LOG_ERR | LOG_DAEMON, "Cannot write pid");
        exit(1);
    }

    forked_func(::std::forward<ARGS>(args)...);
    // Remove pid file and exit when the function returns.
    usleep(30000);
    if (!pid_file_name.empty()) {
        unlink(pid_file_name.c_str());
    }
    exit(0);
    return DAEMON_ERROR; // Placed here to make some compilers happy.
}

}} // namespace patxitron::util

#endif // H_da51e5a8_8577_11e7_8d9f_a7fc6c39368c
