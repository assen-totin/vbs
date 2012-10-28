find | grep c$ | sed s/^..//g > po/POTFILES.in
gettextize -f --no-changelog

aclocal -I m4
automake --add-missing
autoconf
autoheader
