%global php_apiver  %((echo 0; php -i 2>/dev/null | sed -n 's/^PHP API => //p') | tail -1)
%global php_extdir  %(php-config --extension-dir 2>/dev/null || echo "undefined")
%global php_version %(php-config --version 2>/dev/null || echo 0)

Name:           imagick
Version:        3.1.2
Release:        1%{?dist}
Summary:        Provides a wrapper to the ImageMagick library.

Group:          Development/Image
License:        PHP
URL:            http://pecl.php.net/package/imagick
Source0:        http://pecl.php.net/get/imagick.tgz
Source1:		imagick.ini
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:  php-devel
BuildRequires:	ImageMagick-devel
Requires:       php(zend-abi) = %{php_zend_api}
Requires:       php(api) = %{php_apiver}

%description
Imagick is a native php extension to create and modify images using the ImageMagick API.
This extension requires ImageMagick version 6.2.4+ and PHP 5.1.3+.

IMPORTANT: Version 2.x API is not compatible with earlier versions.

%prep
%setup -q -n imagick

%build
%{_bindir}/phpize
%configure
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
make install INSTALL_ROOT=$RPM_BUILD_ROOT

# install configuration
%{__mkdir} -p $RPM_BUILD_ROOT%{_sysconfdir}/php.d
%{__cp} %{SOURCE1} $RPM_BUILD_ROOT%{_sysconfdir}/php.d/imagick.ini

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%doc CREDITS
%config(noreplace) %{_sysconfdir}/php.d/imagick.ini
%{php_extdir}/imagick.so
%{_prefix}/include/php/ext/imagick/
