README.txt for FDMDV2

README.Win32 contains instructions for building on Windows.
README.linux contains instructions for building on Linux.
README.cmake contains instructions for building on Windows or Linux using cmake.

Editing
-------

Please make sure your text editor does not insert tabs, and
used indents of 4 spaces.  The following .emacs code was used to
configure emacs:

(setq-default indent-tabs-mode nil)

(add-hook 'c-mode-common-hook
          (function (lambda ()
                      (setq c-basic-offset 4)
                      )))

