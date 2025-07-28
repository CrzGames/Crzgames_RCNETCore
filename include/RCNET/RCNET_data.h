#ifndef RCNET_DATA_H
#define RCNET_DATA_H

#include <stddef.h> // Required for : size_t

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Enum des types de données pris en charge.
 * 
 * \since Cette enum est disponible depuis RCNET 1.0.0.
 */
typedef enum RCNET_DataType {
    /**
     * Type de données texte.
     */
    RCNET_DATA_TYPE_TEXT,

    /**
     * Type de données brutes / binaires.
     */
    RCNET_DATA_TYPE_RAW_DATA
} RCNET_DataType;

/**
 * \brief Enum des formats d'encodage pris en charge.
 * 
 * \since Cette enum est disponible depuis RCNET 1.0.0.
 */
typedef enum RCNET_EncodeFormat {
    /**
     * Format d'encodage Base64.
     */
    RCNET_ENCODE_FORMAT_BASE64,

    /**
     * Format d'encodage Hexadécimal.
     */
    RCNET_ENCODE_FORMAT_HEX
} RCNET_EncodeFormat;

/**
 * \brief Structure pour stocker des données encodées.
 * 
 * \since Cette structure est disponible depuis RCNET 1.0.0.
 */
typedef struct RCNET_EncodedData {
    /**
     * Pointeur vers les données encodées ou compressées.
     * 
     * \note Doit être libéré par l'appelant.
     */
    char* data;

    /**
     * Taille des données originales.
     * 
     * \note Nécessaire pour la décompression.
     */
    size_t originalSize;

    /**
     * Taille des données après encodage.
     * 
     * \note Nécessaire pour la décompression.
     */
    size_t encodedSize;

    /**
     * Format d'encodage utilisé.
     * 
     * \note Peut être Base64 ou Hex.
     */
    RCNET_EncodeFormat encodeFormat;

    /**
     * Type de données, texte ou données brutes.
     * 
     * \note Utilisé pour déterminer le traitement approprié des données.
     */
    RCNET_DataType dataType;
} RCNET_EncodedData;

/**
 * \since Enum des formats de compression pris en charge.
 * 
 * \since Cette enum est disponible depuis RCNET 1.0.0.
 */
typedef enum RCNET_CompressFormat {
    /**
     * Format de compression LZ4.
     * 
     * \note Utilisé pour la compression rapide et efficace des données.
     */
    RCNET_COMPRESS_FORMAT_LZ4
} RCNET_CompressFormat;

/**
 * \brief Structure pour stocker des données compressées.
 * 
 * \since Cette structure est disponible depuis RCNET 1.0.0.
 */
typedef struct RCNET_CompressedData {
    /**
     * Pointeur vers les données compressées.
     * 
     * \note Doit être libéré par l'appelant.
     */
    unsigned char* data;

    /**
     * Taille des données originales avant compression.
     * Ceci est nécessaire pour la décompression afin de valider 
     * l'intégrité des données décompressées. 
     */
    size_t originalSize;

    /**
     * Taille des données après compression.
     * Ceci est utilisé lors de la décompression pour indiquer 
     * la quantité de données à traiter.
     */
    size_t compressedSize;

    /**
     * Format de compression utilisé pour compresser les données. 
     */
    RCNET_CompressFormat compressFormat;

    /**
     * Type de données d'entrée, texte ou données brutes.
     * Ceci est utilisé pour déterminer le traitement approprié des données.
     */
    RCNET_DataType dataType;
} RCNET_CompressedData;

/**
 * \brief Enum des formats de chiffrement pris en charge.
 * 
 * \since Cette enum est disponible depuis RCNET 1.0.0.
 */
typedef enum RCNET_CipherFormat {
    /**
     * Format de chiffrement AES.
     * 
     * \note Utilisé pour le chiffrement symétrique rapide et sécurisé.
     */
    RCNET_CIPHER_FORMAT_AES,

    /**
     * Format de chiffrement ChaCha20.
     * 
     * \note Utilisé pour le chiffrement symétrique rapide et sécurisé, particulièrement adapté aux environnements mobiles.
     */
    RCNET_CIPHER_FORMAT_CHACHA20,

    /**
     * Format de chiffrement ChaCha20-Poly1305.
     * 
     * \note Utilisé pour le chiffrement symétrique avec authentification intégrée, offrant une sécurité renforcée.
     */
    RCNET_CIPHER_FORMAT_CHACHA20_POLY1305,

    /**
     * TODO: Doit être implémenter -> RSA encryption/decryption
     * 
     * Format de chiffrement RSA.
     * 
     * \note Utilisé pour le chiffrement asymétrique, permettant l'échange sécurisé de clés et la signature numérique.
     */
    RCNET_CIPHER_FORMAT_RSA
} RCNET_CipherFormat;

/**
 * \brief Structure pour stocker des données cryptées.
 * 
 * \note La structure doit être libérée via : rcnet_data_freeSecurity. 
 * Cette fonction s'assure que toutes les données sensibles sont zéroisées avant 
 * de libérer la mémoire allouée, pour aider à prévenir les fuites de données.
 *
 * \since Cette structure est disponible depuis RCNET 1.0.0.
 */
typedef struct RCNET_EncryptedData {
    /**
     * Pointeur vers les données cryptées.
     */
    unsigned char* data;

    /**
     * La passphrase utilisée pour crypter les données.
     */
    char* passphrase;

    /**
     * Pointeur vers le code d'authentification de message (HMAC) utilisé pour 
     * valider l'intégrité des données cryptées.
     */
    unsigned char* hmac;

    /**
     * Taille des données originales avant cryptage.
     */
    size_t originalSize;

    /**
     * Taille des données après cryptage.
     */
    size_t encryptedSize;

    /**
     * Taille du code d'authentification de message (HMAC).
     */
    size_t hmacSize;

    /**
     * Format de chiffrement utilisé pour crypter les données.
     */
    RCNET_CipherFormat cipherFormat;

    /**
     * Type de données cryptées, texte ou données brutes.
     */
    RCNET_DataType dataType;
} RCNET_EncryptedData;

/**
 * \brief Enum des formats de hachage pris en charge.
 * 
 * \since Cette enum est disponible depuis RCNET 1.0.0.
 */
typedef enum RCNET_HashFormat {
    /**
     * Algorithme de hachage MD5, produit un hachage de 128 bits. 
     * Non recommandé pour une utilisation sécurisée en raison de vulnérabilités connues.
     */
    RCNET_HASHING_FORMAT_MD5,

    /**
     * Algorithme de hachage SHA-1, produit un hachage de 160 bits. 
     * Déconseillé pour une utilisation sécurisée en raison de faiblesses connues.
     */
    RCNET_HASHING_FORMAT_SHA1,

    /**
     * Algorithme de hachage SHA-224, une variante de SHA-2, produit un hachage de 224 bits.
     */
    RCNET_HASHING_FORMAT_SHA224,

    /**
     * Algorithme de hachage SHA-256, une variante de SHA-2, produit un hachage de 256 bits, 
     * recommandé pour une utilisation sécurisée.
     */
    RCNET_HASHING_FORMAT_SHA256,

    /**
     * Algorithme de hachage SHA-384, une variante de SHA-2, produit un hachage de 384 bits.
     */
    RCNET_HASHING_FORMAT_SHA384,

    /**
     * Algorithme de hachage SHA-512, une variante de SHA-2, produit un hachage de 512 bits, 
     * offrant une sécurité renforcée.
     */
    RCNET_HASHING_FORMAT_SHA512,

    /**
     * Algorithme de hachage SHA3-224, produit un hachage de 224 bits, basé sur l'algorithme de hachage Keccak.
     */
    RCNET_HASHING_FORMAT_SHA3_224,

    /**
     * Algorithme de hachage SHA3-256, produit un hachage de 256 bits, basé sur l'algorithme de hachage Keccak.
     */
    RCNET_HASHING_FORMAT_SHA3_256,

    /**
     * Algorithme de hachage SHA3-384, produit un hachage de 384 bits, basé sur l'algorithme de hachage Keccak.
     */
    RCNET_HASHING_FORMAT_SHA3_384,

    /**
     * Algorithme de hachage SHA3-512, produit un hachage de 512 bits, basé sur l'algorithme de hachage Keccak.
     */
    RCNET_HASHING_FORMAT_SHA3_512
} RCNET_HashFormat;

/**
 * \brief Encode des données en utilisant le format spécifié.
 *
 * \param {const unsigned char*} data - Pointeur vers les données à encoder.
 * \param {size_t} dataSize - Taille des données à encoder en octets.
 * \param {RCNET_DataType} dataType - Type des données à encoder.
 * \param {RCNET_EncodeFormat} format - Format d'encodage à utiliser.
 * \return {RCNET_EncodedData*} - Pointeur vers un objet RCNET_EncodedData contenant les données encodées et les métadonnées, ou NULL en cas d'échec.
 * 
 * \warning La structure RCNET_EncodedData et son champ `data` doivent être libérés par l'appelant avec `RCNET_safe_free`.
 * 
 * \threadsafety Cette fonction peut être appelée depuis n'importe quel thread.
 * 
 * \since Cette fonction est disponible depuis RCNET 1.0.0.
 * 
 * \see rcnet_data_decode
 */
RCNET_EncodedData* rcnet_data_encode(const unsigned char* data, const size_t dataSize, const RCNET_DataType dataType, const RCNET_EncodeFormat format);

/**
 * \brief Décode des données en utilisant le format spécifié.
 * 
 * \param {const RCNET_EncodedData*} encodedData - Pointeur vers l'objet RCNET_EncodedData contenant les données encodées et les métadonnées nécessaires pour le décodage.
 * \return {unsigned char*} - Pointeur vers les données décodées, ou NULL en cas d'échec.
 * 
 * \warning Le pointeur retourné doit être libéré par l'appelant avec `RCNET_safe_free()`.
 *
 * \threadsafety Cette fonction peut être appelée depuis n'importe quel thread.
 * 
 * \since Cette fonction est disponible depuis RCNET 1.0.0.
 * 
 * \see rcnet_data_encode
 */
unsigned char* rcnet_data_decode(const RCNET_EncodedData* encodedData);

/**
 * \brief Compresse des données en utilisant le format de compression spécifié.
 * 
 * \param {const unsigned char*} data - Pointeur vers les données en clair à compresser.
 * \param {size_t} dataSize - Taille des données en clair en octets.
 * \param {RCNET_DataType} dataType - Type de données fournies, influençant potentiellement l'optimisation de la compression.
 * \param {RCNET_CompressFormat} format - Format de compression à utiliser, défini par l'énumération RCNET_CompressFormat.
 * \return {RCNET_CompressedData*} - Pointeur vers un nouvel objet RCNET_CompressedData contenant les données compressées et les métadonnées, ou NULL en cas de format non supporté ou d'échec de la compression.
 * 
 * \warning La structure RCNET_CompressedData et son champ `data` doivent être libérés par l'appelant avec `RCNET_safe_free`.
 * 
 * \threadsafety Cette fonction peut être appelée depuis n'importe quel thread.
 * 
 * \since Cette fonction est disponible depuis RCNET 1.0.0.
 * 
 * \see rcnet_data_decompress
 */
RCNET_CompressedData* rcnet_data_compress(const unsigned char* data, const size_t dataSize, const RCNET_DataType dataType, const RCNET_CompressFormat format);

/**
 * \brief Décompresse les données contenues dans une structure RCNET_CompressedData.
 * 
 * \param {RCNET_CompressedData*} compressedData - Pointeur vers l'objet RCNET_CompressedData contenant les données compressées et les métadonnées nécessaires pour la décompression.
 * \return {unsigned char*} - Pointeur vers les données décompressées en cas de succès, ou NULL en cas de format non supporté ou d'échec de la décompression.
 * 
 * \warning Le tableau retourné doit être libéré par l'appelant avec `RCNET_safe_free`.
 * 
 * \threadsafety Cette fonction peut être appelée depuis n'importe quel thread.
 * 
 * \since Cette fonction est disponible depuis RCNET 1.0.0.
 * 
 * \see rcnet_data_compress
 */
unsigned char* rcnet_data_decompress(const RCNET_CompressedData* compressedData);

/**
 * \brief Hash une chaîne de caractères en utilisant le format de hachage spécifié. 
 * 
 * Calcule le hash d'une chaîne de caractères en utilisant le format de hashage spécifié.
 * Cette fonction prend une chaîne de caractères en entrée et retourne son hash calculé
 * selon l'algorithme spécifié par `format`. Les formats supportés incluent MD5, SHA1,
 * SHA224, SHA256, SHA384, SHA512, ainsi que les variantes SHA3. La sortie est convertie
 * en une chaîne hexadécimale représentant le hash. OpenSSL est utilisé pour le calcul du hash.
 * 
 * \param data La chaîne de caractères à hasher. Doit être une chaîne C terminée par un null ('\0').
 * \param format L'identifiant du format de hashage, spécifié par l'énumération `RCNET_HashFormat`.
 * 
 * \return Retourne une chaîne de caractères hexadécimale représentant le hash calculé de `data`.
 *         En cas d'échec (par exemple, allocation mémoire impossible, format de hashage non supporté,
 *         ou erreur dans le processus de hashage), la fonction retourne `NULL`.
 * 
 * \warning La chaîne retournée doit être libérée par l'appelant avec `RCNET_safe_free()`.
 * 
 * \threadsafety Cette fonction peut être appelée depuis n'importe quel thread.
 * 
 * \since Cette fonction est disponible depuis RCNET 1.0.0.
 */
char* rcnet_data_hash(const char* data, const RCNET_HashFormat format);

/**
 * \brief Chiffre des données à l'aide d'une passphrase, en utilisant un format de chiffrement spécifié. 
 * 
 * Cette fonction génère une clé de chiffrement dérivée de la passphrase fournie au moyen d'une fonction de dérivation robuste, 
 * en combinant la passphrase avec un sel généré de manière aléatoire pour renforcer la sécurité. 
 * Elle utilise ensuite cette clé pour chiffrer les données et génère un HMAC des données chiffrées 
 * pour permettre un futur test d'intégrité. Les données chiffrées, le sel, l'IV, et le HMAC sont ensuite combinés 
 * en une structure RCNET_EncryptedData qui contient également des métadonnées telles que la taille originale 
 * des données et le format de chiffrement utilisé.
 * 
 * \param {Uint8Array} data - Un pointeur vers les données non chiffrées à chiffrer.
 * \param {number} dataSize - La taille des données en octets.
 * \param {string} passphrase - Une passphrase utilisée pour générer la clé de chiffrement via une fonction de dérivation.
 * \param {RCNET_DataType} dataType - Le type de données fournies, influençant le traitement des données.
 * \param {RCNET_CipherFormat} format - Le format de chiffrement à utiliser, défini par l'énumération RCNET_CipherFormat.
 * \return {RCNET_EncryptedData*} - Un pointeur vers un nouvel objet RCNET_EncryptedData contenant les données chiffrées, le HMAC, le sel, l'IV, et des métadonnées. Retourne NULL en cas d'échec.
 * 
 * \warning La structure RCNET_EncryptedData, son champ `data` et son champ `hmac` doivent être libérés par l'appelant avec `rcnet_data_freeSecurity`.
 * 
 * \threadsafety Cette fonction peut être appelée depuis n'importe quel thread.
 * 
 * \since Cette fonction est disponible depuis RCNET 1.0.0.
 * 
 * \see rcnet_data_decrypt
 * \see rcnet_data_freeSecurity
 */
RCNET_EncryptedData* rcnet_data_encrypt(const unsigned char* data, const size_t dataSize, const char* passphrase, const RCNET_DataType dataType, const RCNET_CipherFormat format);

/**
 * \brief Déchiffre les données contenues dans une structure RCNET_EncryptedData.
 * 
 * Avant le déchiffrement, la fonction vérifie l'intégrité des données chiffrées en utilisant le HMAC fourni. 
 * Si le test d'intégrité réussit, la fonction procède au déchiffrement des données à l'aide de la clé dérivée 
 * de la passphrase et du sel inclus dans l'objet RCNET_EncryptedData. Cette approche garantit que seules les données 
 * non altérées seront déchiffrées, renforçant la sécurité des données échangées.
 *
 * \param {RCNET_EncryptedData*} encryptedData - Un pointeur vers une structure RCNET_EncryptedData contenant les données chiffrées.
 * \return {unsigned char*} - Un pointeur vers les données déchiffrées en cas de succès, ou NULL en cas d'échec.
 * 
 * \warning Le tableau retourné doit être libéré par l'appelant avec `RCNET_safe_free`.
 * 
 * \threadsafety Cette fonction peut être appelée depuis n'importe quel thread.
 * 
 * \since Cette fonction est disponible depuis RCNET 1.0.0.
 * 
 * \see rcnet_data_encrypt
 */
unsigned char* rcnet_data_decrypt(const RCNET_EncryptedData* encryptedData);

/**
 * \brief Libère une structure RCNET_EncryptedData de manière sécurisée. 
 * 
 * Cette fonction s'assure que toutes les données sensibles sont zéroisées avant 
 * de libérer la mémoire allouée, pour aider à prévenir les fuites de données.
 *
 * \param {RCNET_EncryptedData*} encryptedData - Pointeur vers la structure RCNET_EncryptedData à libérer.
 * 
 * \threadsafety Cette fonction peut être appelée depuis n'importe quel thread.
 * 
 * \since Cette fonction est disponible depuis RCNET 1.0.0.
 * 
 * \see rcnet_data_encrypt
 */
void rcnet_data_freeSecurity(RCNET_EncryptedData* encryptedData);

#ifdef __cplusplus
}
#endif  

#endif // RCNET_DATA_H