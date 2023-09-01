#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tson_library.h";

// Структура TsonObject
struct TsonObject {
    // Здесь хранятся пары ключ-значение
    char** keys;
    void** values;
    TsonType* types;
    size_t size;
};
struct TsonArray
{
    void** values;      // Массив указателей на значения
    TsonType* types;    // Массив типов значений
    size_t size;        // Количество элементов в массиве
};


#pragma region Создание и Уничтожение TSON объекта
// Создание объекта TSON
TsonObject* tson_object_create() {
    TsonObject* obj = (TsonObject*)malloc(sizeof(TsonObject));
    if (obj == NULL) {
        // Обработка ошибки выделения памяти
        return NULL;
    }

    obj->keys = NULL;
    obj->values = NULL;
    obj->types = NULL;
    obj->size = 0;

    return obj;
}

// Уничтожение объекта TSON и освобождение памяти
void tson_object_destroy(TsonObject* obj) {
    if (obj == NULL) {
        return;
    }

    // Освобождение памяти для каждого ключа и значения
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

    // Освобождение массивов ключей, значений и типов
    free(obj->keys);
    free(obj->values);
    free(obj->types);

    // Освобождение памяти для самого объекта
    free(obj);
}
#pragma endregion
#pragma region Добавление значений в объект TSON
// Добавление целочисленного значения в объект TSON
void tson_object_set_integer(TsonObject* obj, const char* key, int value) {
    if (obj == NULL || key == NULL) {
        return;
    }

    // Увеличение размера массивов для ключей, значений и типов
    size_t newSize = obj->size + 1;
    char** newKeys = (char**)realloc(obj->keys, newSize * sizeof(char*));
    void** newValues = (void**)realloc(obj->values, newSize * sizeof(void*));
    TsonType* newTypes = (TsonType*)realloc(obj->types, newSize * sizeof(TsonType));

    if (newKeys == NULL || newValues == NULL || newTypes == NULL) {
        // Обработка ошибки выделения памяти
        return;
    }

    // Создание копии ключа
    char* newKey = _strdup(key);
    if (newKey == NULL) {
        // Обработка ошибки выделения памяти
        return;
    }

    // Добавление ключа, значения и типа
    newKeys[obj->size] = newKey;
    newValues[obj->size] = malloc(sizeof(int)); // Выделение памяти под целое число
    *(int*)newValues[obj->size] = value;
    newTypes[obj->size] = TSON_INTEGER;

    // Обновление массивов в объекте TSON
    obj->keys = newKeys;
    obj->values = newValues;
    obj->types = newTypes;
    obj->size = newSize;
}

// Добавление числа с плавающей точкой (float) в объект TSON
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
    newValues[obj->size] = malloc(sizeof(float)); // Выделение памяти под число с плавающей точкой
    *(float*)newValues[obj->size] = value;
    newTypes[obj->size] = TSON_FLOAT;

    obj->keys = newKeys;
    obj->values = newValues;
    obj->types = newTypes;
    obj->size = newSize;
}

// Добавление строки в объект TSON
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
    newValues[obj->size] = _strdup(value); // Копирование строки
    newTypes[obj->size] = TSON_STRING;

    obj->keys = newKeys;
    obj->values = newValues;
    obj->types = newTypes;
    obj->size = newSize;
}

// Добавление логического значения в объект TSON
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
    newValues[obj->size] = malloc(sizeof(int)); // Выделение памяти под логическое значение (int)
    *(int*)newValues[obj->size] = value ? 1 : 0;
    newTypes[obj->size] = TSON_BOOLEAN;

    obj->keys = newKeys;
    obj->values = newValues;
    obj->types = newTypes;
    obj->size = newSize;
}

// Добавление значения null в объект TSON
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
    newValues[obj->size] = NULL; // Просто устанавливаем значение как NULL
    newTypes[obj->size] = TSON_NULL;

    obj->keys = newKeys;
    obj->values = newValues;
    obj->types = newTypes;
    obj->size = newSize;
}

// Добавление объекта в объект TSON
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
    newValues[obj->size] = value; // Просто устанавливаем указатель на переданный объект
    newTypes[obj->size] = TSON_OBJECT;

    obj->keys = newKeys;
    obj->values = newValues;
    obj->types = newTypes;
    obj->size = newSize;
}

// Добавление массива в объект TSON
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
    newValues[obj->size] = value; // Просто устанавливаем указатель на переданный массив
    newTypes[obj->size] = TSON_ARRAY;

    obj->keys = newKeys;
    obj->values = newValues;
    obj->types = newTypes;
    obj->size = newSize;
}
#pragma endregion
#pragma region Получение значений из объекта TSON
// Получение целочисленного значения из объекта TSON
int tson_object_get_integer(const TsonObject* obj, const char* key) {
    if (obj == NULL || key == NULL) {
        return 0; // Возврат значения по умолчанию в случае ошибки
    }

    for (size_t i = 0; i < obj->size; ++i) {
        if (strcmp(obj->keys[i], key) == 0 && obj->types[i] == TSON_INTEGER) {
            return *(int*)obj->values[i];
        }
    }

    return 0; // Возврат значения по умолчанию, если ключ не найден или тип неверный
}

// Получение значения с плавающей точкой из объекта TSON
float tson_object_get_float(const TsonObject* obj, const char* key) {
    if (obj == NULL || key == NULL) {
        return 0.0f; // Возврат значения по умолчанию в случае ошибки
    }

    for (size_t i = 0; i < obj->size; ++i) {
        if (strcmp(obj->keys[i], key) == 0 && obj->types[i] == TSON_FLOAT) {
            return *(float*)obj->values[i];
        }
    }

    return 0.0f; // Возврат значения по умолчанию, если ключ не найден или тип неверный
}

// Получение строки из объекта TSON
const char* tson_object_get_string(const TsonObject* obj, const char* key) {
    if (obj == NULL || key == NULL) {
        return NULL; // Возврат значения NULL в случае ошибки
    }

    for (size_t i = 0; i < obj->size; ++i) {
        if (strcmp(obj->keys[i], key) == 0 && obj->types[i] == TSON_STRING) {
            return (const char*)obj->values[i];
        }
    }

    return NULL; // Возврат значения NULL, если ключ не найден или тип неверный
}

// Получение логического значения из объекта TSON
int tson_object_get_boolean(const TsonObject* obj, const char* key) {
    if (obj == NULL || key == NULL) {
        return 0; // Возврат значения по умолчанию в случае ошибки
    }

    for (size_t i = 0; i < obj->size; ++i) {
        if (strcmp(obj->keys[i], key) == 0 && obj->types[i] == TSON_BOOLEAN) {
            return (*(int*)obj->values[i]) != 0;
        }
    }

    return 0; // Возврат значения по умолчанию, если ключ не найден или тип неверный
}
// Получение объекта из объекта TSON
TsonObject* tson_object_get_object(const TsonObject* obj, const char* key) {
    if (obj == NULL || key == NULL) {
        return NULL; // Возврат NULL в случае ошибки
    }

    for (size_t i = 0; i < obj->size; ++i) {
        if (strcmp(obj->keys[i], key) == 0 && obj->types[i] == TSON_OBJECT) {
            return (TsonObject*)obj->values[i];
        }
    }

    return NULL; // Возврат NULL, если ключ не найден или тип неверный
}

// Получение массива из объекта TSON
TsonArray* tson_object_get_array(const TsonObject* obj, const char* key) {
    if (obj == NULL || key == NULL) {
        return NULL; // Возврат NULL в случае ошибки
    }

    for (size_t i = 0; i < obj->size; ++i) {
        if (strcmp(obj->keys[i], key) == 0 && obj->types[i] == TSON_ARRAY) {
            return (TsonArray*)obj->values[i];
        }
    }

    return NULL; // Возврат NULL, если ключ не найден или тип неверный
}
#pragma endregion

#pragma region Сериализация и десериализация объекта TSON








// Метод для вычисления размера сериализованного объекта TSON
static size_t tson_object_serialized_size(const TsonObject* obj);

// Метод для сериализации объекта TSON в байтовый массив
static void tson_object_serialize_to_buffer(const TsonObject* obj, unsigned char* buffer, size_t* offset);

// Метод для десериализации байтового массива в объект TSON
static TsonObject* tson_object_deserialize_from_buffer(const unsigned char* buffer, size_t* offset);












// Метод для вычисления размера сериализованного массива TSON
size_t tson_array_serialized_size(const TsonArray* arr) {
    size_t totalSize = sizeof(size_t); // Размер для хранения количества элементов

    for (size_t i = 0; i < arr->size; ++i) {
        totalSize += sizeof(TsonType); // Размер типа элемента

        if (arr->types[i] == TSON_INTEGER) {
            totalSize += sizeof(int);
        }
        else if (arr->types[i] == TSON_FLOAT) {
            totalSize += sizeof(float);
        }
        else if (arr->types[i] == TSON_STRING) {
            totalSize += strlen((const char*)arr->values[i]) + 1; // Размер строки и нулевого символа
        }
        else if (arr->types[i] == TSON_BOOLEAN) {
            totalSize += sizeof(int);
        }
        else if (arr->types[i] == TSON_NULL) {
            // Нет данных для добавления
        }
        else if (arr->types[i] == TSON_OBJECT) {
            totalSize += tson_object_serialized_size((TsonObject*)arr->values[i]);
        }
    }

    return totalSize;
}

// Метод для сериализации массива TSON в байтовый массив
void tson_array_serialize_to_buffer(const TsonArray* arr, unsigned char* buffer, size_t* offset) {
    memcpy(buffer + *offset, &(arr->size), sizeof(size_t)); // Сохранение количества элементов
    *offset += sizeof(size_t);

    for (size_t i = 0; i < arr->size; ++i) {
        memcpy(buffer + *offset, &(arr->types[i]), sizeof(TsonType)); // Сохранение типа элемента
        *offset += sizeof(TsonType);

        if (arr->types[i] == TSON_INTEGER) {
            memcpy(buffer + *offset, arr->values[i], sizeof(int)); // Сохранение целочисленного значения
            *offset += sizeof(int);
        }
        else if (arr->types[i] == TSON_FLOAT) {
            memcpy(buffer + *offset, arr->values[i], sizeof(float)); // Сохранение значения с плавающей точкой
            *offset += sizeof(float);
        }
        else if (arr->types[i] == TSON_STRING) {
            size_t strSize = strlen(((char**)arr->values)[i]) + 1;
            memcpy(buffer + *offset, ((char**)arr->values)[i], strSize); // Сохранение строки
            *offset += strSize;
        }
        else if (arr->types[i] == TSON_BOOLEAN) {
            memcpy(buffer + *offset, arr->values[i], sizeof(int)); // Сохранение логического значения
            *offset += sizeof(int);
        }
        else if (arr->types[i] == TSON_NULL) {
            // Нет данных для добавления
        }
        else if (arr->types[i] == TSON_OBJECT) {
            tson_object_serialize_to_buffer(((TsonObject**)arr->values)[i], buffer, offset); // Сериализация вложенного объекта
        }
    }
}

// Метод для десериализации массива TSON из байтового массива
static TsonArray* tson_array_deserialize_from_buffer(const unsigned char* buffer, size_t* offset) {
    size_t size = *((size_t*)(buffer + *offset)); // Получение количества элементов
    *offset += sizeof(size_t);

    TsonArray* arr = tson_array_create(); // Создание массива

    for (size_t i = 0; i < size; ++i) {
        TsonType type = *((TsonType*)(buffer + *offset)); // Получение типа элемента
        *offset += sizeof(TsonType);

        if (type == TSON_INTEGER) {
            int value = *((int*)(buffer + *offset)); // Получение целочисленного значения
            *offset += sizeof(int);
            tson_array_add_integer(arr, value); // Добавление значения в массив
        }
        else if (type == TSON_FLOAT) {
            float value = *((float*)(buffer + *offset)); // Получение значения с плавающей точкой
            *offset += sizeof(float);
            tson_array_add_float(arr, value); // Добавление значения в массив
        }
        else if (type == TSON_STRING) {
            size_t strSize = strlen((const char*)(buffer + *offset)) + 1;
            const char* value = (const char*)(buffer + *offset); // Получение строки
            *offset += strSize;
            tson_array_add_string(arr, value); // Добавление значения в массив
        }
        else if (type == TSON_BOOLEAN) {
            int value = *((int*)(buffer + *offset)); // Получение логического значения
            *offset += sizeof(int);
            tson_array_add_boolean(arr, value); // Добавление значения в массив
        }
        else if (type == TSON_NULL) {
            tson_array_add_null(arr); // Добавление значения null в массив
        }
        else if (type == TSON_OBJECT) {
            TsonObject* nestedObj = tson_object_deserialize_from_buffer(buffer, offset); // Десериализация вложенного объекта
            tson_array_add_object(arr, nestedObj); // Добавление вложенного объекта в массив
        }
    }
    return arr;
}











// Сериализация объекта TSON в байтовый массив
unsigned char* tson_object_serialize(const TsonObject* obj, size_t* size) {
    if (obj == NULL || size == NULL) {
        return NULL;
    }

    // Вычисление размера сериализованного объекта
    *size = tson_object_serialized_size(obj);

    // Выделение памяти под байтовый массив
    unsigned char* buffer = (unsigned char*)malloc(*size);
    if (buffer == NULL) {
        return NULL;
    }

    // Сериализация объекта в байтовый массив
    size_t offset = 0;
    tson_object_serialize_to_buffer(obj, buffer, &offset);

    return buffer;
}

// Десериализация байтового массива в объект TSON
TsonObject* tson_object_deserialize(const unsigned char* data, size_t size) {
    if (data == NULL || size == 0) {
        return NULL;
    }

    size_t offset = 0;
    
    return tson_object_deserialize_from_buffer(data, &offset);
}

// Вспомогательный метод для вычисления размера сериализованного объекта TSON
static size_t tson_object_serialized_size(const TsonObject* obj) {
    size_t totalSize = sizeof(size_t); // Размер для хранения количества элементов

    for (size_t i = 0; i < obj->size; ++i) {
        totalSize += sizeof(TsonType) + sizeof(size_t) + strlen(obj->keys[i]) + 1; // Размер типа, размера ключа и ключа

        if (obj->types[i] == TSON_INTEGER) {
            totalSize += sizeof(int);
        }
        else if (obj->types[i] == TSON_FLOAT) {
            totalSize += sizeof(float);
        }
        else if (obj->types[i] == TSON_STRING) {
            totalSize += strlen((const char*)obj->values[i]) + 1; // Размер строки и нулевого символа
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
// Вспомогательный метод для сериализации объекта TSON в байтовый массив
static void tson_object_serialize_to_buffer(const TsonObject* obj, unsigned char* buffer, size_t* offset) {
    // Сохранение количества элементов в объекте
    memcpy(buffer + *offset, &(obj->size), sizeof(size_t));
    *offset += sizeof(size_t);

    for (size_t i = 0; i < obj->size; ++i) {
        // Сохранение типа элемента
        memcpy(buffer + *offset, &(obj->types[i]), sizeof(TsonType));
        *offset += sizeof(TsonType);

        // Сохранение размера ключа и самого ключа
        size_t keySize = strlen(obj->keys[i]) + 1;
        memcpy(buffer + *offset, &keySize, sizeof(size_t));
        *offset += sizeof(size_t);
        memcpy(buffer + *offset, obj->keys[i], keySize);
        *offset += keySize;

        // Сохранение значения в зависимости от типа
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

// Вспомогательный метод для десериализации байтового массива в объект TSON
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

#pragma region Создание и уничтожение массива TSON

// Создание массива TSON
TsonArray* tson_array_create() {
    TsonArray* arr = (TsonArray*)malloc(sizeof(TsonArray));
    if (arr == NULL) {
        return NULL; // Обработка ошибки выделения памяти
    }

    arr->values = NULL;
    arr->types = NULL;
    arr->size = 0;

    return arr;
}

// Уничтожение массива TSON и освобождение памяти
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
#pragma region Добавление значений в массив TSON
// Добавление целочисленного значения в массив TSON
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

    newValues[arr->size] = malloc(sizeof(int)); // Выделение памяти под целое число
    *(int*)newValues[arr->size] = value;
    newTypes[arr->size] = TSON_INTEGER;

    arr->values = newValues;
    arr->types = newTypes;
    arr->size = newSize;

}

// Добавление значения с плавающей точкой в массив TSON
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

    newValues[arr->size] = malloc(sizeof(float)); // Выделение памяти под число с плавающей точкой
    *(float*)newValues[arr->size] = value;

    newTypes[arr->size] = TSON_FLOAT;

    arr->values = newValues;
    arr->types = newTypes;
    arr->size = newSize;
}

// Добавление строки в массив TSON
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

    newValues[arr->size] = _strdup(value); // Копирование строки
    newTypes[arr->size] = TSON_STRING;

    arr->values = newValues;
    arr->types = newTypes;
    arr->size = newSize;
}

// Добавление логического значения в массив TSON
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

    newValues[arr->size] = malloc(sizeof(int)); // Выделение памяти под логическое значение (int)
    *(int*)newValues[arr->size] = value ? 1 : 0;
    newTypes[arr->size] = TSON_BOOLEAN;

    arr->values = newValues;
    arr->types = newTypes;
    arr->size = newSize;
}
// Добавление значения null в массив TSON
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

    newValues[arr->size] = NULL; // Просто устанавливаем значение как NULL
    newTypes[arr->size] = TSON_NULL;

    arr->values = newValues;
    arr->types = newTypes;
    arr->size = newSize;
}

// Добавление объекта в массив TSON
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

    newValues[arr->size] = value; // Просто устанавливаем указатель на переданный объект
    newTypes[arr->size] = TSON_OBJECT;

    arr->values = newValues;
    arr->types = newTypes;
    arr->size = newSize;
}
#pragma endregion
#pragma region Получение значений из массива TSON
// Получение целочисленного значения из массива TSON
int tson_array_get_integer(const TsonArray* arr, size_t index) {
    if (arr == NULL || index >= arr->size) {
        return 0; // Возврат значения по умолчанию в случае ошибки
    }

    if (arr->types[index] == TSON_INTEGER) {
        return *(int*)arr->values[index];
    }

    return 0; // Возврат значения по умолчанию, если тип неверный
}

// Получение значения с плавающей точкой из массива TSON
float tson_array_get_float(const TsonArray* arr, size_t index) {
    if (arr == NULL || index >= arr->size) {
        return 0.0f; // Возврат значения по умолчанию в случае ошибки
    }

    if (arr->types[index] == TSON_FLOAT) {
        return *(float*)arr->values[index];
    }

    return 0.0f; // Возврат значения по умолчанию, если тип неверный
}

// Получение строки из массива TSON
const char* tson_array_get_string(const TsonArray* arr, size_t index) {
    if (arr == NULL || index >= arr->size) {
        return NULL; // Возврат значения NULL в случае ошибки
    }

    if (arr->types[index] == TSON_STRING) {
        return (const char*)arr->values[index];
    }

    return NULL; // Возврат значения NULL, если тип неверный
}
// Получение логического значения из массива TSON
int tson_array_get_boolean(const TsonArray* arr, size_t index) {
    if (arr == NULL || index >= arr->size) {
        return 0; // Возврат значения по умолчанию в случае ошибки
    }

    if (arr->types[index] == TSON_BOOLEAN) {
        return (*(int*)arr->values[index]) != 0;
    }

    return 0; // Возврат значения по умолчанию, если тип неверный
}

// Получение объекта из массива TSON
TsonObject* tson_array_get_object(const TsonArray* arr, size_t index) {
    if (arr == NULL || index >= arr->size) {
        return NULL; // Возврат значения NULL в случае ошибки
    }

    if (arr->types[index] == TSON_OBJECT) {
        return (TsonObject*)arr->values[index];
    }

    return NULL; // Возврат значения NULL, если тип неверный
}
#pragma endregion
