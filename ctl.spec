Name:       ctl
Version:    0
Release:    noop
Summary:    A computational topology library
License:    BSD-3
BuildArch:  x86_64
Source0:    https://github.com/appliedtopology/%{name}/archive/%{commit0}.tar.gz#/%{name}-%{shortcommit0}.tar.gz
BuildRequires: gcc make git zip
BuildRequires: boost-devel
BuildRequires: ann-devel
BuildRequires: tbb-devel
BuildRequires: metis-devel
BuildRequires: doxygen 

BuildRequires: boost
BuildRequires: ann
BuildRequires: tbb
BuildRequires: metis
BuildRequires: doxygen 

Source: ctl.zip

%description
A computational topology library
%prep
%autosetup -n %{name}-%{shortcommit0}

%build
mkdir build
cd build
cmake ..
make %{?_smp_flags}
 
%install
make install

%files
%{_bindir}/*
%{_libdir}/*

%changelog
