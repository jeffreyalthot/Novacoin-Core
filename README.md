````markdown
# Novacoin-Core (minimal Windows scaffold)

Ce dépôt contient une version minimale et factice de "Novacoin-Core" pour permettre un premier déploiement et compilation sous Windows.

Prérequis
- Windows 10/11
- Visual Studio 2022 (ou équivalent) avec le composant "Desktop development with C++"
- CMake 3.15+

Compilation
1. Ouvrir "x64 Native Tools Command Prompt for VS 2022" ou utiliser l'invite de commande développeur.
2. À la racine du dépôt :

   build.bat

3. L'exécutable sera généré dans build\\Release\\novacoin-node.exe

Usage
L'exécutable courant est une implémentation factice qui affiche un "heartbeat" toutes les 5 secondes et s'arrête quand vous appuyez sur Entrée.

Prochaines étapes recommandées
- Remplacer les composants factices par l'implémentation réelle (consensus, P2P, RPC, stockage de blocks)
- Ajouter des tests unitaires et CI
- Documenter le format des fichiers de configuration
````