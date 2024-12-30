Name:           deepin-shortcut-viewer
Version:        5.0.5
Release:        2
Summary:        Deepin Shortcut Viewer
License:        GPLv3
URL:            https://github.com/linuxdeepin/deepin-shortcut-viewer
Source0:        %{name}_%{version}.orig.tar.xz

BuildRequires:  cmake
BuildRequires:  gcc-c++
BuildRequires:  dtkwidget-devel
BuildRequires:  qt6-qtbase-devel
BuildRequires:  qt6-qtbase-private-devel
BuildRequires:  pkgconfig(Qt6Core)
BuildRequires:  pkgconfig(Qt6Widgets)
BuildRequires:  pkgconfig(Qt6Network)
Provides:       bundled(CuteLogger)

%description
The program displays a shortcut key window when a JSON data is passed.

%prep
%setup -q

%build
%cmake
%cmake_build

%install
%cmake_install

%files
%doc README.md
%license LICENSE
%{_bindir}/%{name}

%changelog
* Thu Jan 31 2024 Package Maintainer <maintainer@deepin.org> - 5.0.5-2
- Update to use Qt6
- Switch build system to CMake

* Thu Jan 31 2019 Fedora Release Engineering <releng@fedoraproject.org> - 1.3.5-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_30_Mass_Rebuild

* Fri Jul 20 2018 mosquito <sensor.wen@gmail.com> - 1.3.5-1
- Update to 1.3.5

* Thu Jul 12 2018 Fedora Release Engineering <releng@fedoraproject.org> - 1.3.4-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_29_Mass_Rebuild

* Fri Feb 16 2018 mosquito <sensor.wen@gmail.com> - 1.3.4-1
- Update to 1.3.4

* Wed Feb 07 2018 Fedora Release Engineering <releng@fedoraproject.org> - 1.3.3-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_28_Mass_Rebuild

* Sat Dec  2 2017 mosquito <sensor.wen@gmail.com> - 1.3.3-1
- Update to 1.3.3

* Mon Aug 21 2017 mosquito <sensor.wen@gmail.com> - 1.3.2-1
- Update to 1.3.2

* Sun Aug 20 2017 mosquito <sensor.wen@gmail.com> - 1.3.1-2
- Use dtkwidget1 library

* Fri Jul 14 2017 mosquito <sensor.wen@gmail.com> - 1.3.1-1.git729e82d
- Update to 1.3.1

* Tue Jan 17 2017 mosquito <sensor.wen@gmail.com> - 1.0.2-1.git760b082
- Update to 1.0.2

* Sun Dec 04 2016 Jaroslav <cz.guardian@gmail.com> Stepanek 1.02-1
- Updated to version 1.02-1

* Mon Oct 10 2016 Jaroslav <cz.guardian@gmail.com> Stepanek 1.01-1
- Initial package build
