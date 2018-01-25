% !TeX spellcheck = fr_FR
\documentclass[a4paper,12pt, french]{article}

\usepackage[francais]{babel} % règles de rédaction fr
\usepackage[utf8]{inputenc} % caractères accentués reconnus utf8
\usepackage[T1]{fontenc}
\usepackage{lmodern}
\usepackage[left=2cm,top=2cm,bottom=2cm,right=2cm]{geometry} % marges
\usepackage{graphicx}
\graphicspath{{screen/}}

\usepackage{enumitem}

% redéfinition maketitle
\makeatletter 
\def\maketitle{  
  \begin{center}  
    \normalfont 
    {\Large \@title} 
    \vskip 0.4cm  
    {\@author}  
    \vskip 0.2cm 
    {\@date}   
  \end{center} 
  }  
\makeatother   

\usepackage{hyperref} % urls
\hypersetup{           %       
    colorlinks=false,  % black links
    pdfborder={0 0 0}, % no ugly border
}

% remplissage champs maketitle %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\title{PG110 : Installation du projet Bombeirb}						
\date{}

%\thispagestyle{empty} %numpage

\begin{document}

\maketitle

Les sections \ref{sect:sdl} et \ref{sect:windows} détaillent respectivement la mise en place d'un environnement de développement pour \texttt{Linux}/\texttt{MacOS} et \texttt{Windows}.  Nous vous proposons d'utiliser l'IDE \texttt{Eclipse} avec le plugin \texttt{CDT} (\textit{C/C++ Development Tooling}). La section \ref{sect:eclipse} décrit les étapes d'importation du projet dans Eclipse.

\section{Linux et MacOS}
\label{sect:sdl}

\subsection{Outils GNU}

Les outils GNU nécessaires au projet (\texttt{GCC}, \texttt{GDB}, \texttt{make}) et la bibliothèque standard C (\texttt{libc}) peuvent être installés avec les paquets \texttt{build-essential} et \texttt{gdb} sous Linux, et par l'installation de \texttt{XCode Command Line Tools} sous MacOS. Depuis la version \texttt{Mountain Lion}, \texttt{lldb} a remplacé \texttt{GDB} pour le débogage. Le débogage en mode graphique sous Eclipse n'est néanmoins possible qu'avec GDB. 

\subsection{SDL}

Le projet utilise les bibliothèque \texttt{SDL} et \texttt{SDL image} en version 1.2 (non compatible avec la version 2) pour l’interface utilisateur et le dessin 2D. Sous Linux, SDL et SDL image peuvent être installés via le gestionnaire de paquet de votre distribution (\texttt{sudo apt-get install libsdl1.2-dev libsdl-image1.2-dev}) sous Debian/Ubuntu/Mint).  Sous MacOS vous pouvez utilisez un gestionnaire de paquet tel que \texttt{homebrew}\footnote{\url{http://brew.sh/}}.

\section{Windows}
\label{sect:windows}

Sous Windows, \texttt{MinGW} (\textit{Minimalist GNU for Windows}) offre une solution pour disposer d'un environnement GNU.

\begin{itemize}[itemsep=5pt]
\item Télécharger la dernière version du netinstaller de \texttt{MinGW} : \\
	\url{http://sourceforge.net/projects/mingw/files/Installer/} => \texttt{mingw-get-setup.exe}

\item Lancer l'installateur et installer \texttt{mingw32-base}, \texttt{msys-base}. De préférence, laisser l'emplacement par défaut (\texttt{C:/MinGW}). Si vous préférez un emplacement alternatif, \textbf{éviter les dossiers parents avec des noms contenant des espaces}.

\item Télécharger l'archive \texttt{SDL-mingw32.zip} à l'adresse : \\
\url{http://www.labri.fr/perso/reveille/pg110/}

\item Extraire le contenu de l'archive dans le dossier MinGW (ajout des include, des bibliothèques SDL et de l'utilitaire \texttt{sdl-config} dans les dossiers \texttt{include}, \texttt{lib} et \texttt{bin} de MinGW).

\end{itemize}

\subsection{cmd \& Path}

La compilation et le lancement du projet dans une invite de commande Windows nécessite l'ajout des répertoires contenant les outils GNU et les bibliothèques dynamiques nécessaires à l'exécution du projet à la variable d'environnement \texttt{Path}. Pour ce faire, accéder aux \texttt{Propriétés systèmes}, puis aux \texttt{Paramètres système avancés}. Ajouter le dossier \texttt{bin} et \texttt{msys/1.0/bin} de \texttt{MinGW}.


\section{Eclipse}
\label{sect:eclipse}

\subsection{Windows : Eclipse et MinGW}

Sous Windows, \texttt{Eclipse CDT} détecte automatiquement \texttt{MinGW} si le répertoire d'installation est celui par défaut (\texttt{C:/MinGW}). Si un autre répertoire d'installation a été utilisé, la création d'un lien symbolique est une solution. Il faut ouvrir une invite de commande, se placer à la racine de \texttt{C:} et exécuter la commande \texttt{mklink /J MinGW  C:/chemin/vers/MinGW}. 

\subsection{Importation du projet}

\begin{itemize}

\item Lancer Eclipse CDT et choisir un chemin vers un nouveau workspace dédié au projet (ex. \texttt{\textasciitilde/workspaces/bombeirb})

\item Menu \texttt{File} / \texttt{Import} / \texttt{Existing Code as Makefile Project} 

\item Renseigner un nom de projet -\texttt{Project Name}-, le chemin vers le dossier du projet -\texttt{Existing Code Location}-.  Sélectionner \texttt{GNU Autotools Toolchain} (Fig. \ref{fig:eclipse-config}). Sur les \textbf{machines de l'Enseirb}, sélectionner \texttt{Cross GCC}.
\\
\begin{figure}[!h]
\begin{center}

\begin{tabular}{cc}
	\includegraphics[width=.45\linewidth]{import-2} &
	\includegraphics[width=.45\linewidth]{run-config}
\end{tabular}
\caption{Configuration Eclipse}
\label{fig:eclipse-config}
\end{center}
\end{figure}

\end{itemize}

\subsection{Run configuration}
Après avoir lancé la compilation du projet, l'exécutable \texttt{bomberman} doit apparaitre dans le dossier \texttt{bin}. Afin de pouvoir lancer son exécution depuis l'interface d'Eclipse, il faut ajouter une configuration de lancement au projet : clic droit sur le projet, \texttt{Run AS, Run configuration...}, clic droit sur \texttt{C/C++ Application} puis \texttt{New}. Renseigner le path vers le binaire : \texttt{bin/bombeirb}. \texttt{Apply} pour appliquer (Fig. \ref{fig:eclipse-config}).



\end{document}
