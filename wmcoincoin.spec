%define name wmcoincoin
%define version 2.3.8b
%define release 1

Summary: wmcoincoin is a stupid dock app for linuxfr-addicted people
Name: %{name}
Version: %{version}
Release: %{release}
Source: %{name}-%{version}.tar.bz2
License: GPL
URL: http://hules.free.fr/wmcoincoin/wmcoincoin.html
Group: Toys
BuildRoot: %{_tmppath}/%{name}-buildroot
Prefix: %{_prefix}

%description
La fonctionnalité centrale (le gros bouton rouge), permet de poster "coin coin!"
sur la tribune libre. Autour de ce bouton, différentes fonctionnalités annexes gravitent:
* Affichage de l'heure du dernier post sur la tribune
* Affichage défilant des titres des dernières news de DaLinuxFrenchPage
Visionnage du contenu des news (sans les commentaires)
* Trollometre incorporé, de qualité professionnelle.
* Un véritable Trolloscope d'un modèle proche de celui qui équipe les services secrets chinois.
* Des ballons d'aide pour toujours plus de convivialité.
* Des stats sur le nombre de personnes qui fréquentent la tribune.
* Le Palmipède Editor qui permet d'éditer le messages/useragent à poster avec un confort maximal.
* Une fonction flamophone, parce que vous le valez bien.
* Le Pinnipède Télétype, un véritable outil de décideur.
Un fichier d'options 'modèle' et largement documenté se trouve avec la doc,
copiez-le dans ~/.wmcoincoin/ puis éditez-le

%prep
%setup

%build
%configure
%make

%install
rm -rf $RPM_BUILD_ROOT
%makeinstall

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,0755)
%data README NEWS COPYING AUTHORS options useragents
%{_bindir}/wmcoincoin
%{_bindir}/wmcoincoin-kde

%changelog
* Sat Jun  1 2002 pouaite <c0in@altern.org> 2.3.8a-1
- version 2.3.8a

* Fri Apr 26 2002 pouaite <c0in@altern.org> 2.3.7b-1
- version 2.3.7b

* Sun Apr 14 2002 pouaite <c0in@altern.org> 2.3.7a-1
- version 2.3.7a

* Sat Mar  9 2002 pouaite <c0in@altern.org> 2.3.6-1
- version 2.3.6

* Mon Feb 18 2002 pouaite <c0in@altern.org> 2.3.5-1
- version 2.3.5

* Sun Jan 20 2002 pouaite <c0in@altern.org> 2.3.4-1
- version 2.3.4

* Sun Dec 16 2001 pommier julien <c0in@altern.org> 2.3.2-1
- version 2.3.2

* Sun Nov 18 2001 pommier julien <c0in@altern.org> 2.2.4beta-1
- passage en v2.2.4 suites aux modifs de dacode

* Sun Nov 18 2001 pommier julien <c0in@altern.org> 2.2.3beta-2
- renomage de remote.php3 en remote.rdf, gestion des tags '<br />'

* Mon Nov 12 2001 pommier julien <c0in@altern.org> 2.2.3-1
- gestion de la messagerie, améliorations diverses

* Tue Oct 23 2001 pommier julien <c0in@altern.org> 2.2beta-1 - réecriture de l'affichage des
news, coup de pied dans le cul de la libwraster, GROSSE modif de la boucle principal (pour éviter
des plantages rares mais bizarres). Ajout d'un TrollDetector(tm)(c)MAPL

* Thu Oct 11 2001 pommier julien <c0in@altern.org> 2.11beta-1
- merge de la branche cygwin de gege, et ajout de petits bugfixes

* Sun Sep 30 2001 pommier julien <c0in@altern.org> 2.1beta-1
- après l'ignoble v2.0, voici venir la v2.1beta et son cortège de fonctionnalités.

* Tue Sep 25 2001 pommier julien <c0in@altern.org> 2.0-2 
- correction d'un bug mineur (certains messages dans le remote.php3
  sont répartis sur plus d'une ligne..)
* Sun Sep 23 2001 pommier julien <c0in@altern.org> 2.0-1
- tentative de création du rpm...


# end of file
