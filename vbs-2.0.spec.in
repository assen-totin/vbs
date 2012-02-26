Summary: Voody Blue Subtitler allows manual synchroniziation of a text file with a video stream, thus creating a subtitles file.
Name: vbs
Version: 1.0
Release: 1
Group: Applications/Multimedia
License: GPL
Source: http://bilbo.online.bg/~assen/vbs/vbs-1.0.tar.gz
BuildRoot: /var/tmp/%{name}-buildroot
Requires: gtk2 >= 2.2.0
Requires: mplayer >= 1.0

%description
Voody Blue Subtitler (aka VBS) is a simple programme that allows you to manually
synchronize a text file with a video stream, thus creating a subtitles file.

%prep
%setup -q
cp vbs.desktop.template vbs.desktop
echo "Icon=/usr/share/vbs/vbs.png"  >> vbs.desktop

%build
make

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/bin
mkdir -p $RPM_BUILD_ROOT/usr/share/vbs
mkdir -p $RPM_BUILD_ROOT/usr/share/applications

install -s -m 755 vbs $RPM_BUILD_ROOT/usr/bin/vbs
install -m 644 vbs.png $RPM_BUILD_ROOT/usr/share/vbs/vbs.png
install -m 644 vbs.desktop $RPM_BUILD_ROOT/usr/share/applications/vbs.desktop

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc README TODO LICENSE CHANGES AUTHOR BUGS

/usr/bin/vbs
/usr/share/vbs/vbs.png
/usr/share/applications/vbs.desktop

%changelog
* Sun Aug 27 2006 Assen Totin <assen@online.bg>
- Initial release.
      
