# Version minimale de CMake requise
cmake_minimum_required(VERSION 3.25.0)

project(rcnet)

# Spécifier la version C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Configuration des drapeaux du compilateur
# -Wall : Active tous les avertissements de base
# -Wextra : Active des avertissements supplémentaires
# -pedantic : Enforce strictement les standards C
# -Wno-unused-parameter : Désactive les avertissements pour les paramètres de fonction non utilisés
# -Wno-error=strict-prototypes : Désactive les erreurs pour les prototypes de fonction stricts
# -Wno-newline-eof : Désactive les avertissements pour les nouvelles lignes à la fin du fichier
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -Wno-unused-parameter -Wno-error=strict-prototypes -Wno-newline-eof")

# Option pour construire l'exécutable d'exemple
option(RCNET_BUILD_EXAMPLE "Build the example executable" ON)

# Chemin vers les dépendances
set(DEPENDENCIES_PATH "${PROJECT_SOURCE_DIR}/dependencies")
set(CRZGAMES_LIBRARIES_PATH "${DEPENDENCIES_PATH}/Crzgames_Libraries")

# Get libraries
find_library(OPENSSL_CRYPTO_LIBRARY NAMES crypto REQUIRED PATHS "${CRZGAMES_LIBRARIES_PATH}/linux/lib/OpenSSL/x64" NO_DEFAULT_PATH)
find_library(OPENSSL_LIBRARY NAMES ssl REQUIRED PATHS "${CRZGAMES_LIBRARIES_PATH}/linux/lib/OpenSSL/x64" NO_DEFAULT_PATH)
find_library(PROTOBUFC_LIBRARY NAMES protobuf-c REQUIRED PATHS "${CRZGAMES_LIBRARIES_PATH}/linux/lib/protobuf-c/x64" NO_DEFAULT_PATH)

# SDL3 : 
# Permet de dire à CMake de ne pas chercher les dépendances dans le système,
# mais de les utiliser à partir du répertoire "dependencies".
add_subdirectory(dependencies/SDL EXCLUDE_FROM_ALL)

# Ajouter les fichiers sources du server de jeu ainsi que les librairies aes, lz4 et monocypher.
file(GLOB_RECURSE SOURCES 
  "${PROJECT_SOURCE_DIR}/src/RCNET/*.c"
  "${PROJECT_SOURCE_DIR}/src/external/monocypher/*.c"
  "${PROJECT_SOURCE_DIR}/src/external/aes/*.c"
  "${PROJECT_SOURCE_DIR}/src/external/lz4/*.c"
)

# Créer une bibliothèque static
add_library(${PROJECT_NAME} 
  STATIC 
  ${SOURCES}
)

# Définir des flags de compilation pour le mode Release (NDEBUG = No Debug, donc Release)
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

# Ajouter les fichiers include du serveur de jeu
target_include_directories(${PROJECT_NAME} PRIVATE
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/include/external"
  "${CRZGAMES_LIBRARIES_PATH}/linux/include"
)

# Link les librairies
target_link_libraries(${PROJECT_NAME}
  SDL3::SDL3
  ${PROTOBUFC_LIBRARY}
  ${OPENSSL_LIBRARY}
  ${OPENSSL_CRYPTO_LIBRARY}
)

if(RCNET_BUILD_EXAMPLE)
  # Ajouter le fichier source pour l'exécutable de test
  file(GLOB_RECURSE EXAMPLE_SOURCES 
    "${PROJECT_SOURCE_DIR}/example/src/*.c"
  )

  # Créer l'exécutable de test
  add_executable(rcnet_example ${EXAMPLE_SOURCES})

  # Ajouter les fichiers include pour l'exécutable de test
  target_include_directories(rcnet_example PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/include/external"
    "${CRZGAMES_LIBRARIES_PATH}/linux/include"
    "${PROJECT_SOURCE_DIR}/example/include"
  )

  # Link les librairies
  target_link_libraries(rcnet_example
    SDL3::SDL3
    ${PROTOBUFC_LIBRARY}
    ${OPENSSL_LIBRARY}
    ${OPENSSL_CRYPTO_LIBRARY}
    ${PROJECT_NAME}
  )
endif()
