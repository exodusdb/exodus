%define baseversion 11.6
%define debug_package %{nil}

Summary: Exodus Multivalue Database Plugin to PostgreSQL
Name: libpgexodus
Version: 9.6.0
Release: 1
Source0: %{name}-%{version}.tar.gz
License: MIT http://www.opensource.org/licenses/mit-license.php
Group: Development/Libraries
Requires: postgresql-libs
BuildRequires: postgresql-devel
BuildRequires: gcc-c++
BuildRoot: /var/tmp/%{name}-%{version}-%{release}-root

%description
Pick/Multivalue database programming using PostgreSQL

%prep
%setup -q

%build
%configure
%{__make}

%install
if [ "$RPM_BUILD_ROOT" != "/var/tmp/%{name}-%{version}-%{release}-root" ]
then
  echo
  echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
  echo @                                                                    @
  echo @  RPM_BUILD_ROOT is not what I expected.  Please clean it yourself. @
  echo @                                                                    @
  echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
  echo
else
  echo Cleaning RPM_BUILD_ROOT: "$RPM_BUILD_ROOT"
  rm -rf "$RPM_BUILD_ROOT"
fi
%{__make} install DESTDIR="$RPM_BUILD_ROOT"

%post -p /sbin/ldconfig

for PGLIBDIR in `ls /usr/lib/postgresql`
do
 ln -s /usr/lib/pgexodus.so /usr/lib/postgresql/$PGLIBDIR/lib/pgexodus.so
done
installexodus-postgresql

%postun -p /sbin/ldconfig

%clean
if [ "$RPM_BUILD_ROOT" != "/var/tmp/%{name}-%{version}-%{release}-root" ]
then
 echo
 echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 echo @                                                                    @
 echo @  RPM_BUILD_ROOT is not what I expected.  Please clean it yourself. @
 echo @                                                                    @
 echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 echo
else
 echo Cleaning RPM_BUILD_ROOT: "$RPM_BUILD_ROOT"
 rm -rf "$RPM_BUILD_ROOT"
fi

%files
%defattr(-,root,root)
%{_libdir}/%{name}*

#%doc /usr/local/info/exodus.info
#%doc %attr(0444,root,root) /usr/local/man/man1/exodus.1
#%doc COPYING AUTHORS README NEWS
