#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tson_library.h";

// ��������� TsonObject
struct TsonObject {
    // ����� �������� ���� ����-��������
    char** keys;
    void** values;
    TsonType* types;
    size_t size;
};
struct TsonArray
{
    void** values;      // ������ ���������� �� ��������
    TsonType* types;    // ������ ����� ��������
    size_t size;        // ���������� ��������� � �������
};


#pragma region �������� � ����������� TSON �������
// �������� ������� TSON
TsonObject* tson_object_create() {
    TsonObject* obj = (TsonObject*)malloc(sizeof(TsonObject));
    if (obj == NULL) {
        // ��������� ������ ��������� ������
        return NULL;
    }

    obj->keys = NULL;
    obj->values = NULL;
    obj->types = NULL;
    obj->size = 0;

    return obj;
}

// ����������� ������� TSON � ������������ ������
void tson_object_destroy(TsonObject* obj) {
    if (obj == NULL) {
        return;
    }

    // ������������ ������ ��� ������� ����� � ��������
    for (size_t i = 0; i < obj->size; i++) {
        free(obj->keys[i]);
        if (obj->types[i] == TSON_OBJECT) {
            tson_object_destroy((TsonObject*)obj->values[i]);
        }
        else if (obj->types[i] == TSON_ARRAY) {
            tson_array_destroy((TsonArray*)obj->values[i]);
        }
        free(obj->values[i]);
    }

    // ������������ �������� ������, �������� � �����
    free(obj->keys);
    free(obj->values);
    free(obj->types);

    // ������������ ������ ��� ������ �������
    free(obj);
}
#pragma endregion
#pragma region ���������� �������� � ������ TSON
// ���������� �������������� �������� � ������ TSON
void tson_object_set_integer(TsonObject* obj, const char* key, int value) {
    if (obj == NULL || key == NULL) {
        return;
    }

    // ���������� ������� �������� ��� ������, �������� � �����
    size_t newSize = obj->size + 1;
    char** newKeys = (char**)realloc(obj->keys, newSize * sizeof(char*));
    void** newValues = (void**)realloc(obj->values, newSize * sizeof(void*));
    TsonType* newTypes = (TsonType*)realloc(obj->types, newSize * sizeof(TsonType));

    if (newKeys == NULL || newValues == NULL || newTypes == NULL) {
        // ��������� ������ ��������� ������
        return;
    }

    // �������� ����� �����
    char* newKey = _strdup(key);
    if (newKey == NULL) {
        // ��������� ������ ��������� ������
        return;
    }

    // ���������� �����, �������� � ����
    newKeys[obj->size] = newKey;
    newValues[obj->size] = malloc(sizeof(int)); // ��������� ������ ��� ����� �����
    *(int*)newValues[obj->size] = value;
    newTypes[obj->size] = TSON_INTEGER;

    // ���������� �������� � ������� TSON
    obj->keys = newKeys;
    obj->values = newValues;
    obj->types = newTypes;
    obj->size = newSize;
}

// ���������� ����� � ��������� ������ (float) � ������ TSON
void tson_object_set_float(TsonObject* obj, const char* key, float value) {
    if (obj == NULL || key == NULL) {
        return;
    }

    size_t newSize = obj->size + 1;
    char** newKeys = (char**)realloc(obj->keys, newSize * sizeof(char*));
    void** newValues = (void**)realloc(obj->values, newSize * sizeof(void*));
    TsonType* newTypes = (TsonType*)realloc(obj->types, newSize * sizeof(TsonType));

    if (newKeys == NULL || newValues == NULL || newTypes == NULL) {
        return;
    }

    char* newKey = _strdup(key);
    if (newKey == NULL) {
        return;
    }
    
    newKeys[obj->size] = newKey;
    newValues[obj->size] = malloc(sizeof(float)); // ��������� ������ ��� ����� � ��������� ������
    *(float*)newValues[obj->size] = value;
    newTypes[obj->size] = TSON_FLOAT;

    obj->keys = newKeys;
    obj->values = newValues;
    obj->types = newTypes;
    obj->size = newSize;
}

// ���������� ������ � ������ TSON
void tson_object_set_string(TsonObject* obj, const char* key, const char* value) {
    if (obj == NULL || key == NULL || value == NULL) {
        return;
    }

    size_t newSize = obj->size + 1;
    char** newKeys = (char**)realloc(obj->keys, newSize * sizeof(char*));
    void** newValues = (void**)realloc(obj->values, newSize * sizeof(void*));
    TsonType* newTypes = (TsonType*)realloc(obj->types, newSize * sizeof(TsonType));

    if (newKeys == NULL || newValues == NULL || newTypes == NULL) {
        return;
    }

    char* newKey = _strdup(key);
    if (newKey == NULL) {
        return;
    }

    newKeys[obj->size] = newKey;
    newValues[obj->size] = _strdup(value); // ����������� ������
    newTypes[obj->size] = TSON_STRING;

    obj->keys = newKeys;
    obj->values = newValues;
    obj->types = newTypes;
    obj->size = newSize;
}

// ���������� ����������� �������� � ������ TSON
void tson_object_set_boolean(TsonObject* obj, const char* key, int value) {
    if (obj == NULL || key == NULL) {
        return;
    }

    size_t newSize = obj->size + 1;
    char** newKeys = (char**)realloc(obj->keys, newSize * sizeof(char*));
    void** newValues = (void**)realloc(obj->values, newSize * sizeof(void*));
    TsonType* newTypes = (TsonType*)realloc(obj->types, newSize * sizeof(TsonType));

    if (newKeys == NULL || newValues == NULL || newTypes == NULL) {
        return;
    }

    char* newKey = _strdup(key);
    if (newKey == NULL) {
        return;
    }

    newKeys[obj->size] = newKey;
    newValues[obj->size] = malloc(sizeof(int)); // ��������� ������ ��� ���������� �������� (int)
    *(int*)newValues[obj->size] = value ? 1 : 0;
    newTypes[obj->size] = TSON_BOOLEAN;

    obj->keys = newKeys;
    obj->values = newValues;
    obj->types = newTypes;
    obj->size = newSize;
}

// ���������� �������� null � ������ TSON
void tson_object_set_null(TsonObject* obj, const char* key) {
    if (obj == NULL || key == NULL) {
        return;
    }

    size_t newSize = obj->size + 1;
    char** newKeys = (char**)realloc(obj->keys, newSize * sizeof(char*));
    void** newValues = (void**)realloc(obj->values, newSize * sizeof(void*));
    TsonType* newTypes = (TsonType*)realloc(obj->types, newSize * sizeof(TsonType));

    if (newKeys == NULL || newValues == NULL || newTypes == NULL) {
        return;
    }

    char* newKey = _strdup(key);
    if (newKey == NULL) {
        return;
    }

    newKeys[obj->size] = newKey;
    newValues[obj->size] = NULL; // ������ ������������� �������� ��� NULL
    newTypes[obj->size] = TSON_NULL;

    obj->keys = newKeys;
    obj->values = newValues;
    obj->types = newTypes;
    obj->size = newSize;
}

// ���������� ������� � ������ TSON
void tson_object_set_object(TsonObject* obj, const char* key, TsonObject* value) {
    if (obj == NULL || key == NULL || value == NULL) {
        return;
    }

    size_t newSize = obj->size + 1;
    char** newKeys = (char**)realloc(obj->keys, newSize * sizeof(char*));
    void** newValues = (void**)realloc(obj->values, newSize * sizeof(void*));
    TsonType* newTypes = (TsonType*)realloc(obj->types, newSize * sizeof(TsonType));

    if (newKeys == NULL || newValues == NULL || newTypes == NULL) {
        return;
    }

    char* newKey = _strdup(key);
    if (newKey == NULL) {
        return;
    }

    newKeys[obj->size] = newKey;
    newValues[obj->size] = value; // ������ ������������� ��������� �� ���������� ������
    newTypes[obj->size] = TSON_OBJECT;

    obj->keys = newKeys;
    obj->values = newValues;
    obj->types = newTypes;
    obj->size = newSize;
}

// ���������� ������� � ������ TSON
void tson_object_set_array(TsonObject* obj, const char* key, TsonArray* value) {
    if (obj == NULL || key == NULL || value == NULL) {
        return;
    }

    size_t newSize = obj->size + 1;
    char** newKeys = (char**)realloc(obj->keys, newSize * sizeof(char*));
    void** newValues = (void**)realloc(obj->values, newSize * sizeof(void*));
    TsonType* newTypes = (TsonType*)realloc(obj->types, newSize * sizeof(TsonType));

    if (newKeys == NULL || newValues == NULL || newTypes == NULL) {
        return;
    }

    char* newKey = _strdup(key);
    if (newKey == NULL) {
        return;
    }

    newKeys[obj->size] = newKey;
    newValues[obj->size] = value; // ������ ������������� ��������� �� ���������� ������
    newTypes[obj->size] = TSON_ARRAY;

    obj->keys = newKeys;
    obj->values = newValues;
    obj->types = newTypes;
    obj->size = newSize;
}
#pragma endregion
#pragma region ��������� �������� �� ������� TSON
// ��������� �������������� �������� �� ������� TSON
int tson_object_get_integer(const TsonObject* obj, const char* key) {
    if (obj == NULL || key == NULL) {
        return 0; // ������� �������� �� ��������� � ������ ������
    }

    for (size_t i = 0; i < obj->size; ++i) {
        if (strcmp(obj->keys[i], key) == 0 && obj->types[i] == TSON_INTEGER) {
            return *(int*)obj->values[i];
        }
    }

    return 0; // ������� �������� �� ���������, ���� ���� �� ������ ��� ��� ��������
}

// ��������� �������� � ��������� ������ �� ������� TSON
float tson_object_get_float(const TsonObject* obj, const char* key) {
    if (obj == NULL || key == NULL) {
        return 0.0f; // ������� �������� �� ��������� � ������ ������
    }

    for (size_t i = 0; i < obj->size; ++i) {
        if (strcmp(obj->keys[i], key) == 0 && obj->types[i] == TSON_FLOAT) {
            return *(float*)obj->values[i];
        }
    }

    return 0.0f; // ������� �������� �� ���������, ���� ���� �� ������ ��� ��� ��������
}

// ��������� ������ �� ������� TSON
const char* tson_object_get_string(const TsonObject* obj, const char* key) {
    if (obj == NULL || key == NULL) {
        return NULL; // ������� �������� NULL � ������ ������
    }

    for (size_t i = 0; i < obj->size; ++i) {
        if (strcmp(obj->keys[i], key) == 0 && obj->types[i] == TSON_STRING) {
            return (const char*)obj->values[i];
        }
    }

    return NULL; // ������� �������� NULL, ���� ���� �� ������ ��� ��� ��������
}

// ��������� ����������� �������� �� ������� TSON
int tson_object_get_boolean(const TsonObject* obj, const char* key) {
    if (obj == NULL || key == NULL) {
        return 0; // ������� �������� �� ��������� � ������ ������
    }

    for (size_t i = 0; i < obj->size; ++i) {
        if (strcmp(obj->keys[i], key) == 0 && obj->types[i] == TSON_BOOLEAN) {
            return (*(int*)obj->values[i]) != 0;
        }
    }

    return 0; // ������� �������� �� ���������, ���� ���� �� ������ ��� ��� ��������
}
// ��������� ������� �� ������� TSON
TsonObject* tson_object_get_object(const TsonObject* obj, const char* key) {
    if (obj == NULL || key == NULL) {
        return NULL; // ������� NULL � ������ ������
    }

    for (size_t i = 0; i < obj->size; ++i) {
        if (strcmp(obj->keys[i], key) == 0 && obj->types[i] == TSON_OBJECT) {
            return (TsonObject*)obj->values[i];
        }
    }

    return NULL; // ������� NULL, ���� ���� �� ������ ��� ��� ��������
}

// ��������� ������� �� ������� TSON
TsonArray* tson_object_get_array(const TsonObject* obj, const char* key) {
    if (obj == NULL || key == NULL) {
        return NULL; // ������� NULL � ������ ������
    }

    for (size_t i = 0; i < obj->size; ++i) {
        if (strcmp(obj->keys[i], key) == 0 && obj->types[i] == TSON_ARRAY) {
            return (TsonArray*)obj->values[i];
        }
    }

    return NULL; // ������� NULL, ���� ���� �� ������ ��� ��� ��������
}
#pragma endregion

#pragma region ������������ � �������������� ������� TSON








// ����� ��� ���������� ������� ���������������� ������� TSON
static size_t tson_object_serialized_size(const TsonObject* obj);

// ����� ��� ������������ ������� TSON � �������� ������
static void tson_object_serialize_to_buffer(const TsonObject* obj, unsigned char* buffer, size_t* offset);

// ����� ��� �������������� ��������� ������� � ������ TSON
static TsonObject* tson_object_deserialize_from_buffer(const unsigned char* buffer, size_t* offset);












// ����� ��� ���������� ������� ���������������� ������� TSON
size_t tson_array_serialized_size(const TsonArray* arr) {
    size_t totalSize = sizeof(size_t); // ������ ��� �������� ���������� ���������

    for (size_t i = 0; i < arr->size; ++i) {
        totalSize += sizeof(TsonType); // ������ ���� ��������

        if (arr->types[i] == TSON_INTEGER) {
            totalSize += sizeof(int);
        }
        else if (arr->types[i] == TSON_FLOAT) {
            totalSize += sizeof(float);
        }
        else if (arr->types[i] == TSON_STRING) {
            totalSize += strlen((const char*)arr->values[i]) + 1; // ������ ������ � �������� �������
        }
        else if (arr->types[i] == TSON_BOOLEAN) {
            totalSize += sizeof(int);
        }
        else if (arr->types[i] == TSON_NULL) {
            // ��� ������ ��� ����������
        }
        else if (arr->types[i] == TSON_OBJECT) {
            totalSize += tson_object_serialized_size((TsonObject*)arr->values[i]);
        }
    }

    return totalSize;
}

// ����� ��� ������������ ������� TSON � �������� ������
void tson_array_serialize_to_buffer(const TsonArray* arr, unsigned char* buffer, size_t* offset) {
    memcpy(buffer + *offset, &(arr->size), sizeof(size_t)); // ���������� ���������� ���������
    *offset += sizeof(size_t);

    for (size_t i = 0; i < arr->size; ++i) {
        memcpy(buffer + *offset, &(arr->types[i]), sizeof(TsonType)); // ���������� ���� ��������
        *offset += sizeof(TsonType);

        if (arr->types[i] == TSON_INTEGER) {
            memcpy(buffer + *offset, arr->values[i], sizeof(int)); // ���������� �������������� ��������
            *offset += sizeof(int);
        }
        else if (arr->types[i] == TSON_FLOAT) {
            memcpy(buffer + *offset, arr->values[i], sizeof(float)); // ���������� �������� � ��������� ������
            *offset += sizeof(float);
        }
        else if (arr->types[i] == TSON_STRING) {
            size_t strSize = strlen(((char**)arr->values)[i]) + 1;
            memcpy(buffer + *offset, ((char**)arr->values)[i], strSize); // ���������� ������
            *offset += strSize;
        }
        else if (arr->types[i] == TSON_BOOLEAN) {
            memcpy(buffer + *offset, arr->values[i], sizeof(int)); // ���������� ����������� ��������
            *offset += sizeof(int);
        }
        else if (arr->types[i] == TSON_NULL) {
            // ��� ������ ��� ����������
        }
        else if (arr->types[i] == TSON_OBJECT) {
            tson_object_serialize_to_buffer(((TsonObject**)arr->values)[i], buffer, offset); // ������������ ���������� �������
        }
    }
}

// ����� ��� �������������� ������� TSON �� ��������� �������
static TsonArray* tson_array_deserialize_from_buffer(const unsigned char* buffer, size_t* offset) {
    size_t size = *((size_t*)(buffer + *offset)); // ��������� ���������� ���������
    *offset += sizeof(size_t);

    TsonArray* arr = tson_array_create(); // �������� �������

    for (size_t i = 0; i < size; ++i) {
        TsonType type = *((TsonType*)(buffer + *offset)); // ��������� ���� ��������
        *offset += sizeof(TsonType);

        if (type == TSON_INTEGER) {
            int value = *((int*)(buffer + *offset)); // ��������� �������������� ��������
            *offset += sizeof(int);
            tson_array_add_integer(arr, value); // ���������� �������� � ������
        }
        else if (type == TSON_FLOAT) {
            float value = *((float*)(buffer + *offset)); // ��������� �������� � ��������� ������
            *offset += sizeof(float);
            tson_array_add_float(arr, value); // ���������� �������� � ������
        }
        else if (type == TSON_STRING) {
            size_t strSize = strlen((const char*)(buffer + *offset)) + 1;
            const char* value = (const char*)(buffer + *offset); // ��������� ������
            *offset += strSize;
            tson_array_add_string(arr, value); // ���������� �������� � ������
        }
        else if (type == TSON_BOOLEAN) {
            int value = *((int*)(buffer + *offset)); // ��������� ����������� ��������
            *offset += sizeof(int);
            tson_array_add_boolean(arr, value); // ���������� �������� � ������
        }
        else if (type == TSON_NULL) {
            tson_array_add_null(arr); // ���������� �������� null � ������
        }
        else if (type == TSON_OBJECT) {
            TsonObject* nestedObj = tson_object_deserialize_from_buffer(buffer, offset); // �������������� ���������� �������
            tson_array_add_object(arr, nestedObj); // ���������� ���������� ������� � ������
        }
    }
    return arr;
}











// ������������ ������� TSON � �������� ������
unsigned char* tson_object_serialize(const TsonObject* obj, size_t* size) {
    if (obj == NULL || size == NULL) {
        return NULL;
    }

    // ���������� ������� ���������������� �������
    *size = tson_object_serialized_size(obj);

    // ��������� ������ ��� �������� ������
    unsigned char* buffer = (unsigned char*)malloc(*size);
    if (buffer == NULL) {
        return NULL;
    }

    // ������������ ������� � �������� ������
    size_t offset = 0;
    tson_object_serialize_to_buffer(obj, buffer, &offset);

    return buffer;
}

// �������������� ��������� ������� � ������ TSON
TsonObject* tson_object_deserialize(const unsigned char* data, size_t size) {
    if (data == NULL || size == 0) {
        return NULL;
    }

    size_t offset = 0;
    
    return tson_object_deserialize_from_buffer(data, &offset);
}

// ��������������� ����� ��� ���������� ������� ���������������� ������� TSON
static size_t tson_object_serialized_size(const TsonObject* obj) {
    size_t totalSize = sizeof(size_t); // ������ ��� �������� ���������� ���������

    for (size_t i = 0; i < obj->size; ++i) {
        totalSize += sizeof(TsonType) + sizeof(size_t) + strlen(obj->keys[i]) + 1; // ������ ����, ������� ����� � �����

        if (obj->types[i] == TSON_INTEGER) {
            totalSize += sizeof(int);
        }
        else if (obj->types[i] == TSON_FLOAT) {
            totalSize += sizeof(float);
        }
        else if (obj->types[i] == TSON_STRING) {
            totalSize += strlen((const char*)obj->values[i]) + 1; // ������ ������ � �������� �������
        }
        else if (obj->types[i] == TSON_BOOLEAN) {
            totalSize += sizeof(int);
        }
        else if (obj->types[i] == TSON_OBJECT) {
            totalSize += tson_object_serialized_size((TsonObject*)obj->values[i]);
        }
        else if (obj->types[i] == TSON_ARRAY) {
            totalSize += tson_array_serialized_size((TsonArray*)obj->values[i]);
        }
    }

    return totalSize;
}
// ��������������� ����� ��� ������������ ������� TSON � �������� ������
static void tson_object_serialize_to_buffer(const TsonObject* obj, unsigned char* buffer, size_t* offset) {
    // ���������� ���������� ��������� � �������
    memcpy(buffer + *offset, &(obj->size), sizeof(size_t));
    *offset += sizeof(size_t);

    for (size_t i = 0; i < obj->size; ++i) {
        // ���������� ���� ��������
        memcpy(buffer + *offset, &(obj->types[i]), sizeof(TsonType));
        *offset += sizeof(TsonType);

        // ���������� ������� ����� � ������ �����
        size_t keySize = strlen(obj->keys[i]) + 1;
        memcpy(buffer + *offset, &keySize, sizeof(size_t));
        *offset += sizeof(size_t);
        memcpy(buffer + *offset, obj->keys[i], keySize);
        *offset += keySize;

        // ���������� �������� � ����������� �� ����
        if (obj->types[i] == TSON_INTEGER) {
            memcpy(buffer + *offset, obj->values[i], sizeof(int));
            *offset += sizeof(int);
        }
        else if (obj->types[i] == TSON_FLOAT) {
            memcpy(buffer + *offset, obj->values[i], sizeof(float));
            *offset += sizeof(float);
        }
        else if (obj->types[i] == TSON_STRING) {
            size_t strSize = strlen((const char*)obj->values[i]) + 1;
            memcpy(buffer + *offset, obj->values[i], strSize);
            *offset += strSize;
        }
        else if (obj->types[i] == TSON_BOOLEAN) {
            memcpy(buffer + *offset, obj->values[i], sizeof(int));
            *offset += sizeof(int);
        }
        else if (obj->types[i] == TSON_OBJECT) {
            tson_object_serialize_to_buffer((TsonObject*)obj->values[i], buffer, offset);
        }
        else if (obj->types[i] == TSON_ARRAY) {
            tson_array_serialize_to_buffer((TsonArray*)obj->values[i], buffer, offset);
        }
    }
}

// ��������������� ����� ��� �������������� ��������� ������� � ������ TSON
static TsonObject* tson_object_deserialize_from_buffer(const unsigned char* buffer, size_t* offset) {
    size_t size = *((size_t*)(buffer + *offset));
    *offset += sizeof(size_t);

    TsonObject* obj = tson_object_create();

    for (size_t i = 0; i < size; ++i) {
        TsonType type = *((TsonType*)(buffer + *offset));
        *offset += sizeof(TsonType);

        size_t keySize = *((size_t*)(buffer + *offset));
        *offset += sizeof(size_t);
        const char* key = (const char*)(buffer + *offset);
        *offset += keySize;

        if (type == TSON_INTEGER) {
            int value = *((int*)(buffer + *offset));
            *offset += sizeof(int);
            tson_object_set_integer(obj, key, value);
        }
        else if (type == TSON_FLOAT) {
            float value = *((float*)(buffer + *offset));
            *offset += sizeof(float);
            tson_object_set_float(obj, key, value);
        }
        else if (type == TSON_STRING) {
            const char* value = (const char*)(buffer + *offset);
            *offset += strlen(value) + 1;
            tson_object_set_string(obj, key, value);
        }
        else if (type == TSON_BOOLEAN) {
            int value = *((int*)(buffer + *offset));
            *offset += sizeof(int);
            tson_object_set_boolean(obj, key, value);
        }
        else if (type == TSON_OBJECT) {
            TsonObject* nestedObj = tson_object_deserialize_from_buffer(buffer, offset);
            tson_object_set_object(obj, key, nestedObj);
        }
        else if (type == TSON_ARRAY) {
            TsonArray* nestedArr = tson_array_deserialize_from_buffer(buffer, offset);
            tson_object_set_array(obj, key, nestedArr);
        }
    }

    return obj;
}

#pragma endregion

#pragma region �������� � ����������� ������� TSON

// �������� ������� TSON
TsonArray* tson_array_create() {
    TsonArray* arr = (TsonArray*)malloc(sizeof(TsonArray));
    if (arr == NULL) {
        return NULL; // ��������� ������ ��������� ������
    }

    arr->values = NULL;
    arr->types = NULL;
    arr->size = 0;

    return arr;
}

// ����������� ������� TSON � ������������ ������
void tson_array_destroy(TsonArray* arr) {
    if (arr == NULL) {
        return;
    }

    for (size_t i = 0; i < arr->size; ++i) {
        if (arr->types[i] == TSON_OBJECT) {
            tson_object_destroy((TsonObject*)arr->values[i]);
        }
        else if (arr->types[i] == TSON_ARRAY) {
            tson_array_destroy((TsonArray*)arr->values[i]);
        }
        free(arr->values[i]);
    }

    free(arr->values);
    free(arr->types);
    free(arr);
}
#pragma endregion
#pragma region ���������� �������� � ������ TSON
// ���������� �������������� �������� � ������ TSON
void tson_array_add_integer(TsonArray* arr, int value) {
    if (arr == NULL) {
        return;
    }

    size_t newSize = arr->size + 1;
    void** newValues = (void**)realloc(arr->values, newSize * sizeof(void*));
    TsonType* newTypes = (TsonType*)realloc(arr->types, newSize * sizeof(TsonType));

    if (newValues == NULL || newTypes == NULL) {
        return;
    }

    newValues[arr->size] = malloc(sizeof(int)); // ��������� ������ ��� ����� �����
    *(int*)newValues[arr->size] = value;
    newTypes[arr->size] = TSON_INTEGER;

    arr->values = newValues;
    arr->types = newTypes;
    arr->size = newSize;

}

// ���������� �������� � ��������� ������ � ������ TSON
void tson_array_add_float(TsonArray* arr, float value) {
    if (arr == NULL) {
        return;
    }

    size_t newSize = arr->size + 1;
    void** newValues = (void**)realloc(arr->values, newSize * sizeof(void*));
    TsonType* newTypes = (TsonType*)realloc(arr->types, newSize * sizeof(TsonType));

    if (newValues == NULL || newTypes == NULL) {
        return;
    }

    newValues[arr->size] = malloc(sizeof(float)); // ��������� ������ ��� ����� � ��������� ������
    *(float*)newValues[arr->size] = value;

    newTypes[arr->size] = TSON_FLOAT;

    arr->values = newValues;
    arr->types = newTypes;
    arr->size = newSize;
}

// ���������� ������ � ������ TSON
void tson_array_add_string(TsonArray* arr, const char* value) {
    if (arr == NULL || value == NULL) {
        return;
    }

    size_t newSize = arr->size + 1;
    void** newValues = (void**)realloc(arr->values, newSize * sizeof(void*));
    TsonType* newTypes = (TsonType*)realloc(arr->types, newSize * sizeof(TsonType));

    if (newValues == NULL || newTypes == NULL) {
        return;
    }

    newValues[arr->size] = _strdup(value); // ����������� ������
    newTypes[arr->size] = TSON_STRING;

    arr->values = newValues;
    arr->types = newTypes;
    arr->size = newSize;
}

// ���������� ����������� �������� � ������ TSON
void tson_array_add_boolean(TsonArray* arr, int value) {
    if (arr == NULL) {
        return;
    }

    size_t newSize = arr->size + 1;
    void** newValues = (void**)realloc(arr->values, newSize * sizeof(void*));
    TsonType* newTypes = (TsonType*)realloc(arr->types, newSize * sizeof(TsonType));

    if (newValues == NULL || newTypes == NULL) {
        return;
    }

    newValues[arr->size] = malloc(sizeof(int)); // ��������� ������ ��� ���������� �������� (int)
    *(int*)newValues[arr->size] = value ? 1 : 0;
    newTypes[arr->size] = TSON_BOOLEAN;

    arr->values = newValues;
    arr->types = newTypes;
    arr->size = newSize;
}
// ���������� �������� null � ������ TSON
void tson_array_add_null(TsonArray* arr) {
    if (arr == NULL) {
        return;
    }

    size_t newSize = arr->size + 1;
    void** newValues = (void**)realloc(arr->values, newSize * sizeof(void*));
    TsonType* newTypes = (TsonType*)realloc(arr->types, newSize * sizeof(TsonType));

    if (newValues == NULL || newTypes == NULL) {
        return;
    }

    newValues[arr->size] = NULL; // ������ ������������� �������� ��� NULL
    newTypes[arr->size] = TSON_NULL;

    arr->values = newValues;
    arr->types = newTypes;
    arr->size = newSize;
}

// ���������� ������� � ������ TSON
void tson_array_add_object(TsonArray* arr, TsonObject* value) {
    if (arr == NULL || value == NULL) {
        return;
    }

    size_t newSize = arr->size + 1;
    void** newValues = (void**)realloc(arr->values, newSize * sizeof(void*));
    TsonType* newTypes = (TsonType*)realloc(arr->types, newSize * sizeof(TsonType));

    if (newValues == NULL || newTypes == NULL) {
        return;
    }

    newValues[arr->size] = value; // ������ ������������� ��������� �� ���������� ������
    newTypes[arr->size] = TSON_OBJECT;

    arr->values = newValues;
    arr->types = newTypes;
    arr->size = newSize;
}
#pragma endregion
#pragma region ��������� �������� �� ������� TSON
// ��������� �������������� �������� �� ������� TSON
int tson_array_get_integer(const TsonArray* arr, size_t index) {
    if (arr == NULL || index >= arr->size) {
        return 0; // ������� �������� �� ��������� � ������ ������
    }

    if (arr->types[index] == TSON_INTEGER) {
        return *(int*)arr->values[index];
    }

    return 0; // ������� �������� �� ���������, ���� ��� ��������
}

// ��������� �������� � ��������� ������ �� ������� TSON
float tson_array_get_float(const TsonArray* arr, size_t index) {
    if (arr == NULL || index >= arr->size) {
        return 0.0f; // ������� �������� �� ��������� � ������ ������
    }

    if (arr->types[index] == TSON_FLOAT) {
        return *(float*)arr->values[index];
    }

    return 0.0f; // ������� �������� �� ���������, ���� ��� ��������
}

// ��������� ������ �� ������� TSON
const char* tson_array_get_string(const TsonArray* arr, size_t index) {
    if (arr == NULL || index >= arr->size) {
        return NULL; // ������� �������� NULL � ������ ������
    }

    if (arr->types[index] == TSON_STRING) {
        return (const char*)arr->values[index];
    }

    return NULL; // ������� �������� NULL, ���� ��� ��������
}
// ��������� ����������� �������� �� ������� TSON
int tson_array_get_boolean(const TsonArray* arr, size_t index) {
    if (arr == NULL || index >= arr->size) {
        return 0; // ������� �������� �� ��������� � ������ ������
    }

    if (arr->types[index] == TSON_BOOLEAN) {
        return (*(int*)arr->values[index]) != 0;
    }

    return 0; // ������� �������� �� ���������, ���� ��� ��������
}

// ��������� ������� �� ������� TSON
TsonObject* tson_array_get_object(const TsonArray* arr, size_t index) {
    if (arr == NULL || index >= arr->size) {
        return NULL; // ������� �������� NULL � ������ ������
    }

    if (arr->types[index] == TSON_OBJECT) {
        return (TsonObject*)arr->values[index];
    }

    return NULL; // ������� �������� NULL, ���� ��� ��������
}
#pragma endregion
