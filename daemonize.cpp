/**
 * @file daemonize.cpp
 * 
 * Daemonizes (puts it in background) the app
 *
 * @date 2017-08-20
 * @uthor Francisco J. Lazur
 */

#include "daemonize.hpp"
#include "safesave.hpp"
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

using namespace std;

namespace patxitron {




daemonize_result daemonize(string const& daemon_name
                          ,function<void ()> forked_func)
{
    pid_t own = getpid();
    string pid_file_name;
    if (!daemon_name.empty()) {
        // Check if there is another instance running and save our pid to a file
        pid_file_name += "/var/run/";
        pid_file_name += daemon_name;
        pid_file_name += ".pid";
        {
            ifstream pidfile(pid_file_name.c_str());
            if (pidfile.is_open()) {
                pid_t other = -1;
                pidfile >> other;
                if (other > 1 && kill(other, 0) == 0) {
                    syslog(LOG_NOTICE | LOG_USER, "An instance is already running");
                    return DAEMON_ALREADY_RUNNING;
                }
            }
        }
    }
    pid_file_name = safeSave(own, pid_file_name);
    if (pid_file_name.empty()) {
        syslog(LOG_ERR | LOG_USER, "Cannot write PID");
        return DAEMON_ERROR;
    }

    if (pid_t pid = fork()) {
        if (pid > 0) {
            // Loop for at most 1 second waiting for daemon to start
            size_t counter = 100;
            while (counter > 0) {
                ifstream pidfile(pid_file_name.c_str());
                if (pidfile.is_open()) {
                    pid_t other = own;
                    pidfile >> other;
                    if (other > 0 && other != own && kill(other, 0) == 0) {
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
    // We don't want to restrict the permissions on files created by the
    // daemon, so the mask is cleared.
    umask(0);

    // A second fork ensures the process cannot acquire a controlling terminal.
    if (pid_t pid = fork()) {
        if (pid > 0) {
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
    if (open("/dev/null", O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) != 1) {
        syslog(LOG_ERR | LOG_DAEMON, "Unable to open /dev/null as stdout: %m");
        exit(1);
    }

    // Also send standard error to null.
    if (dup(1) != 2) {
        syslog(LOG_ERR | LOG_DAEMON, "Unable to dup stdout to stderr: %m");
        exit(1);
    }

    // write our pid to file
    own = getpid();
    pid_file_name = safeSave(own, pid_file_name);
    if (pid_file_name.empty()) {
        syslog(LOG_ERR | LOG_DAEMON, "Cannot write PID");
        exit(1);
    }

    forked_func();
    exit(0); // Exit when the function returns.
    return DAEMON_ERROR; // Placed here to make some compilers happy.
}

} // namespace patxitron
