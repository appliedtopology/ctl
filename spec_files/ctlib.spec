Name: ctlib
Version: 0.1
Release: 4%{?dist}
Summary: A fast generic C++ library for applied and computational topology
Group: Development/Libraries
License: GPLv3+	
URL: http://www.ctl.appliedtopology.org
Source0: http://ctl.appliedtopology.org/r/v%{version}.tgz
BuildRequires: cmake
BuildRequires: tbb-devel
BuildRequires: boost-devel
BuildRequires: metis-devel

%description 
A fast generic C++ library for applied and computational topology

%package devel
Summary: Computational Topology Library Headers 
Group: Development/Libraries
Requires: %{name} = %{version}-%{release}
Provides: %{name}-static = %{version}-%{release}

%description devel
C++ Header files for the CTL packages.


%package docs
Summary: Computational Topology Documentation package
Group: Development/Libraries
Requires: %{name}%{?_isa} = %{version}-%{release}

%description docs
Documentation in HTML format for CTL. This is the
same as the documentation on the CTL website
(http://ctl.appliedtopology.org/doc/0_01_0)

%package examples
Summary: Source examples for CTL
Requires: %{name}-devel = %{version}-%{release}

%description examples 
This package contains example source files distributed with CTL

%prep
%setup -q -n ctl
#this file has not been written in v0.1 
#so we remove it to avoid rpmlint warnings
rm ctl/zigzag/persistence.h
cmake .

%build
make %{?_smp_mflags}

%install
mkdir -p %{buildroot}%{_bindir}
mkdir -p %{buildroot}%{_includedir}
mkdir -p %{buildroot}%{_docdir}
mkdir -p %{buildroot}%{_mandir}/man1
mkdir -p %{buildroot}%{_docdir}/examples

make install

cp -pr bin/* %{buildroot}%{_bindir}
cp -pr include/* %{buildroot}%{_includedir}
cp -pr doc/* %{buildroot}%{_docdir}
cp -pr man/* %{buildroot}%{_mandir}/man1
cp -pr examples/* %{buildroot}%{_docdir}/examples

%files  
%{_bindir}/*
%doc %{_mandir}/man1/*

%files devel
%{_includedir}/*
%doc %{_docdir}/LICENSE

%files docs
%doc %{_docdir}/AUTHORS
%doc %{_docdir}/ct.bib  
%doc %{_docdir}/README

%files examples
%doc %{_docdir}/examples/*

%changelog
* Sun Apr 27 2014 Ryan H. Lewis <me@ryanlewis.net> - 0.1-4
- Added example files to spec.

* Sun Apr 27 2014 Ryan H. Lewis <me@ryanlewis.net> - 0.1-3
- I am now packaging manual pages, removed -static per #fedora-devel

* Sun Apr 27 2014 Ryan H. Lewis <me@ryanlewis.net> - 0.1-2
- Added rm for empty zigzag persistence header. 

* Sun Apr 27 2014 Ryan H. Lewis <me@ryanlewis.net> - 0.1-1
- Initial Package 
