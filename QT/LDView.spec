%if 0%{?suse_version}
%define dist .openSUSE%(echo %{suse_version} | sed 's/0$//')
%endif

%if 0%{?sles_version}
%define dist .SUSE%(echo %{sles_version} | sed 's/0$//')
%endif

%if "%{vendor}" == "obs://build.opensuse.org/home:pbartfai"
%define opensuse_bs 1
%endif

%if 0%{?centos_ver}
%define centos_version %{centos_ver}00
%endif

Summary: 3D Viewer for LDraw models
Name: ldview
%if 0%{?suse_version} || 0%{?sles_version}
Group: Productivity/Graphics/Viewers
%endif
%if 0%{?mdkversion} || 0%{?rhel_version} 
Group: Graphics
%endif
%if 0%{?fedora} || 0%{?centos_version}
Group: Amusements/Graphics
%endif
Version: 4.2
Release: 1%{?dist}
%if 0%{?mdkversion} || 0%{?rhel_version} || 0%{?fedora} || 0%{?centos_version}
License: GPLv2+
%endif
%if 0%{?suse_version} || 0%{?sles_version}
License: GPL-2.0+
%endif
URL: http://ldview.sourceforge.net
Vendor: Travis Cobbs <ldview@gmail.com>
Packager: Peter Bartfai <pbartfai@stardust.hu>
BuildRoot: %{_builddir}/%{name}
Requires: unzip

%if 0%{?fedora} || 0%{?rhel_version} || 0%{?centos_version}
%if ( 0%{?centos_version}>=600 || 0%{?rhel_version}>=600 || 0%{?fedora} )
BuildRequires: qt-devel
%endif
BuildRequires: boost-devel, cvs, kdebase-devel
BuildRequires: gcc-c++, libpng-devel, make
%endif

%if 0%{?fedora} || 0%{?centos_version}
BuildRequires: mesa-libOSMesa-devel
%endif

%if 0%{?rhel_version}
%define without_osmesa 1
%define tinyxml_static 1
%define gl2ps_static   1
%endif
Source0: LDView.tar.gz

%if 0%{?fedora}
BuildRequires: libjpeg-turbo-devel, tinyxml-devel, gl2ps-devel
%if 0%{?opensuse_bs}
BuildRequires: samba4-libs
%endif
%endif

%if 0%{?centos_version}
%define tinyxml_static 1
%define gl2ps_static   1
%endif

%if 0%{?suse_version}
%kde4_runtime_requires
BuildRequires: libqt4-devel, boost-devel, cmake, libkde4-devel, update-desktop-files
Requires(pre): gconf2
%if 0%{?suse_version} > 1210
BuildRequires: gl2ps-devel
%else
%define gl2ps_static   1
%endif
%if 0%{?suse_version} > 1220
BuildRequires: glu-devel
%endif
%define tinyxml_static 1
%if 0%{?opensuse_bs}
BuildRequires:	-post-build-checks
%endif
%endif

%if 0%{?sles_version}
%define tinyxml_static 1
%if 0%{?opensuse_bs}
BuildRequires:	-post-build-checks
%endif
Requires(post): desktop-file-utils
%endif

%if 0%{?mdkversion}
BuildRequires: libqt4-devel, boost-devel, cmake, kdelibs4-devel
%define gl2ps_static   1
# For openSUSE Build Service
%if 0%{?opensuse_bs}
%if (0%{?mdkversion} != 200910) && (0%{?mdkversion} != 201000)
BuildRequires: kde-l10n-en_GB
%endif
BuildRequires: aspell-en, myspell-en_US
%endif
%define tinyxml_static 1
%define without_osmesa 1
%endif

%if ( 0%{?centos_version}<600 && 0%{?centos_version}>=500 ) || ( 0%{?rhel_version}<600 && 0%{?rhel_version}>=500 )
BuildRequires: qt4-devel
%endif

%description
LDView is a real-time 3D viewer for displaying LDraw models using 
hardware-accelerated 3D graphics. It was written using OpenGL, so should be 
accelerated on any video card which provides full OpenGL 3D acceleration 
(so-called mini-drivers are not likely to work). It should also work on 
other video cards using OpenGL software rendering, albeit at a much slower 
speed. For information on LDraw, please visit http://www.ldraw.org, the 
centralized LDraw information site. 

The program can read LDraw DAT files as well as MPD files. It then allows 
you to rotate the model around to any angle with the mouse. A fast computer 
or a video card with T&L support (Transform & Lighting) is strongly 
recommended for displaying complex models. 

%prep
cd $RPM_SOURCE_DIR
if [ -s %{SOURCE0} ] ; then
	if [ -d LDView ] ; then rm -rf LDView ; fi
	tar zxf %{SOURCE0}
else
	cvs -q -z3 -d:pserver:anonymous@ldview.cvs.sourceforge.net/cvsroot/ldview co LDView
fi

%build
%define is_kde4 %(which kde4-config >/dev/null && echo 1 || echo 0)
%if 0%{?tinyxml_static}
cd $RPM_SOURCE_DIR/LDView/3rdParty/tinyxml
make -f Makefile.pbartfai TESTING="%{optflags}"
cp -f libtinyxml.a ../../lib
export RPM_OPT_FLAGS="$RPM_OPT_FLAGS -I../3rdParty/tinyxml"
%endif
%if 0%{?gl2ps_static}
cd $RPM_SOURCE_DIR/LDView/gl2ps
make TESTING="%{optflags}"
cp -f libgl2ps.a ../lib
cp -f gl2ps.h ../include
export RPM_OPT_FLAGS="$RPM_OPT_FLAGS -I../gl2ps"
%endif
cd $RPM_SOURCE_DIR/LDView/QT
%ifarch i386 i486 i586 i686
%define qplatform linux-g++-32
%endif
%ifarch x86_64
%define qplatform linux-g++-64
%endif
%if ( 0%{?centos_version}<600 && 0%{?centos_version}>=500 ) || ( 0%{?rhel_version}<600 && 0%{?rhel_version}>=500 )
if [ -x %{_libdir}/qt4/bin/qmake ] ; then
export PATH=%{_libdir}/qt4/bin:$PATH
fi
%ifarch x86_64
export RPM_OPT_FLAGS="$RPM_OPT_FLAGS -I%{_libdir}/qt4/include"
%endif
%endif
if which qmake-qt4 >/dev/null 2>/dev/null ; then
	qmake-qt4 -spec %{qplatform}
else
	qmake -spec %{qplatform}
fi
make clean
make TESTING="$RPM_OPT_FLAGS"
if which lrelease-qt4 >/dev/null 2>/dev/null ; then
	lrelease-qt4 LDView.pro
else
	lrelease LDView.pro
fi
strip LDView
%if "%{without_osmesa}" != "1"
cd ../OSMesa
make clean
make TESTING="$RPM_OPT_FLAGS"
%endif
cd ../QT/kde
if [ -d build ]; then rm -rf build ; fi
mkdir -p build
cd build
if cmake -DCMAKE_C_FLAGS_RELEASE="%{optflags}" \
-DCMAKE_CXX_FLAGS_RELEASE="%{optflags}" \
-DCMAKE_INSTALL_PREFIX=`kde4-config --prefix` .. ; then
make
fi

%install
cd $RPM_SOURCE_DIR/LDView/QT
mkdir -p $RPM_BUILD_ROOT%{_datadir}/ldview
mkdir -p $RPM_BUILD_ROOT%{_bindir}
install -d $RPM_BUILD_ROOT%{_datadir}/ldview
install -m 755 LDView $RPM_BUILD_ROOT%{_bindir}/LDView
%if "%{without_osmesa}" != "1"
strip ../OSMesa/ldview
install -m 755 ../OSMesa/ldview $RPM_BUILD_ROOT%{_bindir}/ldview
%endif
install -m 644 ../Textures/SansSerif.fnt \
$RPM_BUILD_ROOT%{_datadir}/ldview/SansSerif.fnt
install -m 644 ../Help.html $RPM_BUILD_ROOT%{_datadir}/ldview/Help.html
install -m 644 ../Readme.txt $RPM_BUILD_ROOT%{_datadir}/ldview/Readme.txt
install -m 644 ../ChangeHistory.html \
				$RPM_BUILD_ROOT%{_datadir}/ldview/ChangeHistory.html
install -m 644 ../license.txt \
				$RPM_BUILD_ROOT%{_datadir}/ldview/license.txt
install -m 644 ../m6459.ldr $RPM_BUILD_ROOT%{_datadir}/ldview/m6459.ldr
install -m 644 ../8464.mpd $RPM_BUILD_ROOT%{_datadir}/ldview/8464.mpd 
install -m 644 ../LDViewMessages.ini \
				$RPM_BUILD_ROOT%{_datadir}/ldview/LDViewMessages.ini
cat ../LDExporter/LDExportMessages.ini >> \
				$RPM_BUILD_ROOT%{_datadir}/ldview/LDViewMessages.ini
install -m 644 ../Translations/German/LDViewMessages.ini \
				$RPM_BUILD_ROOT%{_datadir}/ldview/LDViewMessages_de.ini
install -m 644 ../Translations/Italian/LDViewMessages.ini \
				$RPM_BUILD_ROOT%{_datadir}/ldview/LDViewMessages_it.ini
install -m 644 ../Translations/Czech/LDViewMessages.ini \
			    $RPM_BUILD_ROOT%{_datadir}/ldview/LDViewMessages_cz.ini
install -m 644 ../Translations/Hungarian/LDViewMessages.ini \
				$RPM_BUILD_ROOT%{_datadir}/ldview/LDViewMessages_hu.ini
install -m 644 todo.txt $RPM_BUILD_ROOT%{_datadir}/ldview/todo.txt
install -m 644 ldview_en.qm $RPM_BUILD_ROOT%{_datadir}/ldview/ldview_en.qm
install -m 644 ldview_de.qm $RPM_BUILD_ROOT%{_datadir}/ldview/ldview_de.qm
install -m 644 ldview_it.qm $RPM_BUILD_ROOT%{_datadir}/ldview/ldview_it.qm
install -m 644 ldview_cz.qm $RPM_BUILD_ROOT%{_datadir}/ldview/ldview_cz.qm
install -m 644 ../LDExporter/LGEO.xml \
			   $RPM_BUILD_ROOT%{_datadir}/ldview/LGEO.xml
mkdir -p $RPM_BUILD_ROOT%{_datadir}/mime-info/
mkdir -p $RPM_BUILD_ROOT%{_datadir}/mime/packages/
mkdir -p $RPM_BUILD_ROOT%{_datadir}/application-registry/
mkdir -p $RPM_BUILD_ROOT%{_datadir}/applications/
mkdir -p $RPM_BUILD_ROOT%{_datadir}/thumbnailers/
mkdir -p $RPM_BUILD_ROOT%{_datadir}/pixmaps/
mkdir -p $RPM_BUILD_ROOT%{_datadir}/icons/gnome/32x32/mimetypes
mkdir -p $RPM_BUILD_ROOT%{_sysconfdir}/gconf/schemas
install -m 644 desktop/ldraw.mime $RPM_BUILD_ROOT%{_datadir}/mime-info/ldraw.mime
install -m 644 desktop/ldraw.xml  \
				$RPM_BUILD_ROOT%{_datadir}/mime/packages/ldraw.xml
install -m 644 desktop/ldraw.keys $RPM_BUILD_ROOT%{_datadir}/mime-info/ldraw.keys
install -m 644 desktop/ldview.applications \
			$RPM_BUILD_ROOT%{_datadir}/application-registry/ldview.applications
install -m 644 desktop/ldraw.desktop \
				$RPM_BUILD_ROOT%{_datadir}/applications/ldraw.desktop
install -m 644 desktop/ldview.thumbnailer \
				$RPM_BUILD_ROOT%{_datadir}/thumbnailers/ldview.thumbnailer
install -m 755 desktop/ldraw-thumbnailer \
				$RPM_BUILD_ROOT%{_bindir}/ldraw-thumbnailer
install -m 644 images/LDViewIcon.png \
				$RPM_BUILD_ROOT%{_datadir}/pixmaps/gnome-ldraw.png
install -m 644 images/LDViewIcon.png $RPM_BUILD_ROOT%{_datadir}/icons/gnome/32x32/mimetypes/gnome-mime-application-x-ldraw.png
install -m 644 images/LDViewIcon.png $RPM_BUILD_ROOT%{_datadir}/icons/gnome/32x32/mimetypes/gnome-mime-application-x-multipart-ldraw.png
install -m 644 desktop/ldraw.schemas \
			$RPM_BUILD_ROOT%{_sysconfdir}/gconf/schemas/ldraw.schemas
mkdir -p $RPM_BUILD_ROOT%{_datadir}/kde4/services
install -m 644 kde/ldviewthumbnailcreator.desktop \
		$RPM_BUILD_ROOT%{_datadir}/kde4/services/ldviewthumbnailcreator.desktop
if [ -f kde/build/lib/ldviewthumbnail.so ] ; then
mkdir -p $RPM_BUILD_ROOT/%{_libdir}/kde4
install -m 644 kde/build/lib/ldviewthumbnail.so \
				$RPM_BUILD_ROOT/%{_libdir}/kde4/ldviewthumbnail.so
strip $RPM_BUILD_ROOT/%{_libdir}/kde4/ldviewthumbnail.so
fi
%if 0%{?suse_version}
%suse_update_desktop_file ldraw Graphics
%endif

%files
%if 0%{?sles_version} || 0%{?suse_version}
%defattr(-,root,root)
%endif
%{_bindir}/LDView
%{_datadir}/ldview
%if %{is_kde4}
%dir %{_libdir}/kde4
%{_libdir}/kde4/ldviewthumbnail.so
%endif
%dir %{_datadir}/kde4/services
%dir /etc/gconf/schemas
%dir /usr/share/icons/gnome
%dir %{_datadir}/icons/gnome/32x32
%dir %{_datadir}/icons/gnome/32x32/mimetypes
%dir %{_datadir}/mime-info
%dir %{_datadir}/application-registry
%{_datadir}/kde4/services/ldviewthumbnailcreator.desktop
%{_datadir}/mime-info/ldraw.mime
%{_datadir}/mime/packages/ldraw.xml
%{_datadir}/mime-info/ldraw.keys
%{_datadir}/application-registry/ldview.applications
%{_datadir}/applications/ldraw.desktop
%{_datadir}/thumbnailers/ldview.thumbnailer
%{_bindir}/ldraw-thumbnailer
%{_datadir}/pixmaps/gnome-ldraw.png
%{_datadir}/icons/gnome/32x32/mimetypes/gnome-mime-application-x-ldraw.png
%{_datadir}/icons/gnome/32x32/mimetypes/gnome-mime-application-x-multipart-ldraw.png
%config(noreplace) %{_sysconfdir}/gconf/schemas/ldraw.schemas

%clean
rm -rf $RPM_BUILD_ROOT

%post
%if 0%{?suse_version} >= 1140
%desktop_database_post
%endif
update-mime-database  /usr/share/mime >/dev/null || true
update-desktop-database || true
export GCONF_CONFIG_SOURCE="$(gconftool-2 --get-default-source)"
gconftool-2 --makefile-install-rule /etc/gconf/schemas/ldraw.schemas >/dev/null || true
NAUTILUS=`pidof nautilus`
if [ -n "$NAUTILUS" ] ; then kill -HUP $NAUTILUS ; fi 

%postun
%if 0%{?suse_version} >= 1140
%desktop_database_postun
%endif
update-mime-database /usr/share/mime >/dev/null || true
update-desktop-database || true

%pre
if [ "$1" -gt 1 ] ; then
export GCONF_CONFIG_SOURCE="$(gconftool-2 --get-default-source)"
if [ -f /etc/gconf/schemas/ldraw.schemas ] ; then
gconftool-2 --makefile-uninstall-rule /etc/gconf/schemas/ldraw.schemas >/dev/null || true
fi
fi

%preun
if [ "$1" -eq 0 ] ; then
export GCONF_CONFIG_SOURCE="$(gconftool-2 --get-default-source)"
if [ -f /etc/gconf/schemas/ldraw.schemas ] ; then
gconftool-2 --makefile-uninstall-rule /etc/gconf/schemas/ldraw.schemas >/dev/null || true
fi
fi

%if "%{without_osmesa}" != "1"
%package osmesa
Summary: OSMesa port of LDView for servers without X11
%if 0%{?suse_version} || 0%{?sles_version}
Group: Productivity/Graphics/Viewers
%endif
%if 0%{?mdkversion} || 0%{?rhel_version} 
Group: Graphics
%endif
%if 0%{?fedora} || 0%{?centos_version}
Group: Amusements/Graphics
%endif
%description osmesa
OSMesa port of LDView for servers without X11
No hardware acceleration is used.

%files osmesa
%if 0%{?sles_version} || 0%{?suse_version}
%defattr(-,root,root)
%endif
%{_bindir}/ldview
%endif

%changelog
* Tue Sep 25 2012 - pbartfai (at) stardust.hu
- Changelog added
- Moved files from /usr/local to /usr
- General cleanup for rpmlint checkups
