# Quick Start
 
This guide will help you set up a basic project structure. This is the minimum starter for the RCNET engine so you can start creating your game server !

## 🛠 Tech Stack
- C++ (Language)
- A C++17 compiler (GCC 8.0 or newer)
- doctest (Unit tests)
- CI / CD (Github actions)
- CMake (Build script)
- Compiler (GCC)

<br /><br />


## 📂 Global Project Structure

Voici une représentation de l'arborescence du projet :

::: details Click me to view
```
├─ .github/                        # Dossier qui contient les fichiers de CI/CD pour GitHub Actions
│  ├─ build_deploy_production.yml/ # CI/CD : Environment Production (branch: main)
│  ├─ build_deploy_staging.yml/    # CI/CD : Environment staging (branch: staging)
│  ├─ test_deploy-release.yml/     # CI/CD : Deploy new version Release Notes in Github and tag new version for before run production (branch: main)
├─ build-scripts/                  # Le dossier qui contient tout les scripts de build
│  ├─ generate-project-production/
│  │  ├─ linux.sh                  # Bundle .AppImage
│  ├─ generate-project-development/
│  │  ├─ linux.sh                  # Generate project Makefile with CMake for game project
│  ├─ run-project-development/
│  │  ├─ linux.sh                  # Run project en mode Debug
├─ dependencies/                   # Submodules pour les librairies pour chaque plateformes.
├─ dist/                           # Builds du jeu pour chaque plateforme (exclu du versioning)
├─ include/                        # Dossier include pour les fichiers include du jeu
│  ├─ game.h                       # Fichier .h des prototype pour la logique de votre jeu
│  ├─ unit-tests.h                 # (Facultatif) Fichier .h du prototype pour la fonction pour lancer les tests unitaires
├─ platforms/                      # Dossier qui contient des choses spécifique pour toute les plateformes
│  ├─ linux/                       # appimage.cmake pour le bundle d'un .AppImage
├─ src/                            # Dossier des fichiers source du jeu
│  ├─ game.cpp                     # Fichier d'implémentation pour la logique de votre jeu
│  ├─ main.cpp                     # Le point d'entrée de votre application
│  ├─ unit-tests.cpp               # (Facultatif) Pour les tests unitaires avec Doctest
├─ .gitignore                      # Permet d'éviter de versionner certains fichiers
├─ .gitmodules                     # Contient les path vers les dependencies (submodules)
├─ CHANGELOG.md                    # Journal des modifications des diffèrente versions
├─ CMakeLists.txt                  # Script de construction
├─ README.md                       # Documentation de votre projet à personnaliser
├─ release-please-config.json      # Configuration pour la génération des versions dans la CI
├─ version.txt                     # Fichier de version du jeu
```
:::

::: warning IMPORTANT
Assurez-vous de bien organiser vos fichiers et répertoires selon cette structure pour faciliter la maintenance et le déploiement de votre projet.
:::

<br /><br />


## 🚀 Main Project Structure

Your project should include at least the following files:

- `main.cpp`: The entry point of your application.
- `game.h`: Header file containing declarations for your game's callback functions.
- `game.cpp`: Implementation file for your game's logic.
- `unit-tests.h`: (Optional) Header file containing declarations for unit testing with Doctest.
- `unit-tests.cpp`: (Optional) For unit testing with Doctest.

<br />

### main.cpp

The `main.cpp` file should include the RC2D header, unit-tests header and your game header.

::: warning IMPORTANT
Ensure the `main` function prototype is exactly `int main(int argc, char* argv[])` for SDL2_main compatibility.
:::

```cpp
#include <rc2d/RC2D.h>
#include "game.h"
#include "unit-tests.h"

int main(int argc, char* argv[])
{
#ifndef DOCTEST_CONFIG_DISABLE
    if (rc2d_tests_runAllTests(argc, argv) != 0) return -1;
#endif

    RC2D_Callbacks myGameCallbacks = {};
    myGameCallbacks.rc2d_unload = rc2d_unload;
    myGameCallbacks.rc2d_load = rc2d_load;
    myGameCallbacks.rc2d_update = rc2d_update;
    myGameCallbacks.rc2d_draw = rc2d_draw;
    // Implement other callbacks...

    rc2d_run(&myGameCallbacks);

    return 0;
}
```

### game.h

Declare your game's callback functions and any other game-specific functionality.

::: details Click me to view the code
```cpp
#ifndef _GAME_H_
#define _GAME_H_

void rc2d_unload(void);
void rc2d_load(void);
void rc2d_update(double dt);
void rc2d_draw(void);

// Implement other callbacks...

#endif // _GAME_H_
```
:::

### game.cpp

Implement your game's logic and callback functions.

::: details Click me to view the code
```cpp
#include "game.h"

RC2D_Image image;

void rc2d_unload(void)
{
    rc2d_graphics_freeImage(image);
}

void rc2d_load(void)
{
    image = rc2d_graphics_newImage("images/test.png");
}

void rc2d_update(double dt)
{

}

void rc2d_draw(void)
{
    rc2d_graphics_draw(image, 300, 200);
}

// Implement other callbacks...
```
:::

### unit-tests.h

(Optional) Header file containing declarations for unit testing with Doctest.

::: details Click me to view the code
```cpp
#ifndef _UNIT_TESTS_H_
#define _UNIT_TESTS_H_

int rc2d_tests_runAllTests(int argc, char* argv[]);

#endif // _UNIT_TESTS_H_
```
:::

### unit-tests.cpp

(Optional) For projects using unit testing with Doctest.

::: details Click me to view the code
```cpp
#include "unit-tests.h"
#ifndef DOCTEST_CONFIG_DISABLE
#define DOCTEST_CONFIG_IMPLEMENT
#endif
#include <doctest/doctest.h>

// Run all tests with Doctest and return the result
int rc2d_tests_runAllTests(int argc, char* argv[])
{
    doctest::Context context; // Initialisez le contexte
    context.applyCommandLine(argc, argv); // Appliquez les arguments de la ligne de commande
    int test_result = context.run(); // Exécutez les tests

    if(context.shouldExit() || test_result != 0) 
    {
        return test_result; // Termine l'exécution si Doctest demande de quitter ou si les tests échouent
    }

    return 0;
}

TEST_CASE("Example test case") {
    int x = 1;
    CHECK(x == 1); // Test success
    CHECK(x == 2); // Test error
}
```
:::

<br /><br />


## ✨ À propos de RCNET

### 🕹️ RCNETCore
Une bibliothèque écrite en C pur, fournissant l'ensemble des fonctionnalités nécessaires pour développer des server de jeu. Cette librairie constitue le cœur du moteur de server RCNET et conçue pour être extrêmement performante et flexible.

### 📦 RCNETFramework
RCNETFramework, ou le "server template", est construit au-dessus de RCNETCore et offre une structure de projet et des outils pour démarrer rapidement le développement de votre server de jeu. RC2DFramework ajoute quelques librairies en plus comme : nlohmann (json), cpp-httplib et doctest (unit tests) par rapport au package RCNETCore, qui oblige RCNETFramework à passé en C++ à cause de ces nouvelles librairies.

<br /><br />


## 🎮 Commencer le Développement
Pour commencer le développement avec RCNET, suivez les étapes décrites dans la section Structure Globale du Projet pour configurer votre environnement de développement, puis utilisez RCNETCore et RCNETFramework comme bases pour développer la logique de votre server de jeu.

Bonne création avec RCNET !