# Novacoin-Core (minimal Windows scaffold)

Ce dépôt contient une version minimale et factice de "Novacoin-Core" pour permettre un premier déploiement et compilation sous Windows.

Nouveautés dans ce commit
- Ajout d'une simple gestion de configuration (src/config.*)
- Ajout d'un logger minimal écrit dans le répertoire de données (src/logging.*)
- Intégration du logger et config dans Novacoin (src/novacoin.*)

Prérequis
- Windows 10/11
- Visual Studio 2022 (ou équivalent) avec le composant "Desktop development with C++"
- CMake 3.15+

Fichiers de configuration
- novacoin.conf (optionnel) : fichier texte avec des paires clé=valeur. Supportés : data_dir, log_file
  Exemple :

  data_dir=C:\Users\You\AppData\Roaming\Novacoin
  log_file=C:\Users\You\AppData\Roaming\Novacoin\novacoin.log

Compilation
1. Ouvrir "x64 Native Tools Command Prompt for VS 2022" ou utiliser l'invite de commande développeur.
2. À la racine du dépôt :

   build.bat

3. L'exécutable sera généré dans build\\Release\\novacoin-node.exe

Usage
L'exécutable courant est une implémentation factice qui affiche un "heartbeat" toutes les 5 secondes et s'arrête quand vous appuyez sur Entrée. Les logs sont écrits dans le fichier défini par la configuration (par défaut %APPDATA%\\Novacoin\\novacoin.log).

Prochaines étapes recommandées
- Remplacer les composants factices par l'implémentation réelle (consensus, P2P, RPC, stockage de blocks)
- Ajouter des tests unitaires et CI
- Documenter le format des fichiers de configuration