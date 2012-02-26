Summary: Voody Blue Subtitler suit 
Name: vbs
Version: 2.0
Release: 1
Group: Applications/Multimedia
License: GPL
Source: http://bilbo.online.bg/~assen/vbs/vbs-2.0.tar.gz
BuildRoot: /var/tmp/%{name}-buildroot
Requires: gtk2 >= 2.2.0
Requires: mplayer >= 1.0

%description
Voody Blue Subtitler (aka VBS) is a suit of programmes to manually
synchronize a text file with a video stream, thus creating a subtitles file, 
or to play back sucha file, or to stream it over the network.

%prep
%setup -q

%build
./configure --prefix=/usr
make

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/bin
mkdir -p $RPM_BUILD_ROOT/usr/share/vbs
mkdir -p $RPM_BUILD_ROOT/usr/share/applications

install -s -m 755 vbsm/vbsm $RPM_BUILD_ROOT/usr/bin/vbsm
install -s -m 755 vbss/vbss $RPM_BUILD_ROOT/usr/bin/vbss
install -s -m 755 vbsc/vbsc $RPM_BUILD_ROOT/usr/bin/vbsc
install -s -m 755 vbsd/vbsd $RPM_BUILD_ROOT/usr/sbin/vbsd
install -m 644 common/vbs.png $RPM_BUILD_ROOT/usr/share/vbs/vbs.png
install -m 644 vbsm/vbsm.desktop $RPM_BUILD_ROOT/usr/share/applications/vbsm.desktop
install -m 644 vbss/vbss.desktop $RPM_BUILD_ROOT/usr/share/applications/vbss.desktop
install -m 644 vbsc/vbsc.desktop $RPM_BUILD_ROOT/usr/share/applications/vbsc.desktop

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc README TODO LICENSE CHANGES AUTHOR BUGS

/usr/bin/vbsm
/usr/bin/vbss
/usr/bin/vbsc
/usr/sbin/vbsd
/usr/share/vbs/vbs.png
/usr/share/applications/vbsm.desktop
/usr/share/applications/vbss.desktop
/usr/share/applications/vbsc.desktop

%changelog
* Sun Aug 27 2006 Assen Totin <assen@online.bg>
- Initial release.

* Thu Mar 1 2012 Assen Totin <assen.totin@gmail.com>
- Release 2.0
      
