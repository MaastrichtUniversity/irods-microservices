#!/bin/bash

# detect operating system
UNAMERESULTS=`uname`
if [ -f "/etc/centos-release" ]; then       # CentOS (the order matters here, since CentOS has both /etc/centos- and /etc/redhat-release files)
    DETECTEDOS="CentOS"
elif [ -f "/etc/redhat-release" ]; then     # RHEL and Fedora
    DETECTEDOS="RedHatCompatible"
elif [ -f "/etc/SuSE-release" ]; then       # SuSE
    DETECTEDOS="SuSE"
elif [ -f "/etc/arch-release" ]; then       # ArchLinux
    DETECTEDOS="ArchLinux"
elif [ -f "/etc/lsb-release" ]; then        # Ubuntu
    DETECTEDOS="Ubuntu"
elif [ -f "/etc/debian_version" ]; then     # Debian
    DETECTEDOS="Debian"
elif [ -f "/usr/bin/sw_vers" ]; then        # MacOSX
    DETECTEDOS="MacOSX"
elif [ "$UNAMERESULTS" == "SunOS" ]; then   # Solaris
    DETECTEDOS="Solaris"
fi

echo "$DETECTEDOS"

exit 0