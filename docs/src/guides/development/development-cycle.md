
# 🔄 Development Cycle

This section details the steps involved in the development cycle of projects using the RCNET game server engine, from generating your project to running it.

## 🔧 Generate Project

::: danger IMPORTANT
Assurez-vous que votre environnement de développement est correctement configuré. <br />
Voir : [Prérequis](https://librcnet.crzcommon2.com/guides/getting-started/prerequisites.html)
:::

### 🐧 Linux
::: details Click me to view
```bash
./build-scripts/generate-project-development/linux.sh
```
Ce script est spécifique à Linux, en générant un environnement de build pour l'architecture x86_64 et en configurant les environnements de build pour Debug et Release.
:::

<br />

:::tip IMPORTANT
Chaque script de génération de projet effectue les opérations suivantes :
- Création des dossiers nécessaires pour les builds, situé à la racine du repository : `./dist/`
- Configuration des environnements de build spécifiques à chaque plateforme, pour les modes `Debug` et `Release`.
- Nettoyage des builds précédents avant de commencer une nouvelle compilation.
:::

<br /><br /><br /><br />


## ▶️ Run Project
:::danger IMPORTANT
Pour pouvoir run le projet via le script, il faut avoir OBLIGATOIREMENT générer le projet via les explications ci-dessus.
:::

### 🐧 Linux
::: details Click me to view
```bash
./build-scripts/run-project-development/linux.sh
```
Ce script lance votre projet Linux, en préparant l'environnement pour une `exécution` en mode `Debug`. Il nettoie et reconstruit le projet et `exécute l'application`.
:::