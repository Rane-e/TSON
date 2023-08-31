#ifndef TSON_LIBRARY_H
#define TSON_LIBRARY_H

#include <stddef.h>

// Типы данных TSON
typedef enum {
    TSON_INTEGER,
    TSON_FLOAT,
    TSON_STRING,
    TSON_BOOLEAN,
    TSON_NULL,
    TSON_OBJECT,
    TSON_ARRAY
} TsonType;

// Определение структур для TSON объекта и массива
typedef struct TsonObject TsonObject;
typedef struct TsonArray TsonArray;

// Создание и уничтожение объекта TSON
TsonObject* tson_object_create();
void tson_object_destroy(TsonObject* obj);

// Добавление значений в объект TSON
void tson_object_set_integer(TsonObject* obj, const char* key, int value);
void tson_object_set_float(TsonObject* obj, const char* key, float value);
void tson_object_set_string(TsonObject* obj, const char* key, const char* value);
void tson_object_set_boolean(TsonObject* obj, const char* key, int value);
void tson_object_set_null(TsonObject* obj, const char* key);
void tson_object_set_object(TsonObject* obj, const char* key, TsonObject* value);
void tson_object_set_array(TsonObject* obj, const char* key, TsonArray* value);

// Получение значений из объекта TSON
int tson_object_get_integer(const TsonObject* obj, const char* key);
float tson_object_get_float(const TsonObject* obj, const char* key);
const char* tson_object_get_string(const TsonObject* obj, const char* key);
int tson_object_get_boolean(const TsonObject* obj, const char* key);
TsonObject* tson_object_get_object(const TsonObject* obj, const char* key);
TsonArray* tson_object_get_array(const TsonObject* obj, const char* key);

// Сериализация и десериализация объекта TSON
unsigned char* tson_object_serialize(const TsonObject* obj, size_t* size);
TsonObject* tson_object_deserialize(const unsigned char* data, size_t size);

// Создание и уничтожение массива TSON
TsonArray* tson_array_create();
void tson_array_destroy(TsonArray* arr);

// Добавление значений в массив TSON
void tson_array_add_integer(TsonArray* arr, int value);
void tson_array_add_float(TsonArray* arr, float value);
void tson_array_add_string(TsonArray* arr, const char* value);
void tson_array_add_boolean(TsonArray* arr, int value);
void tson_array_add_null(TsonArray* arr);
void tson_array_add_object(TsonArray* arr, TsonObject* value);

// Получение значений из массива TSON
int tson_array_get_integer(const TsonArray* arr, size_t index);
float tson_array_get_float(const TsonArray* arr, size_t index);
const char* tson_array_get_string(const TsonArray* arr, size_t index);
int tson_array_get_boolean(const TsonArray* arr, size_t index);
TsonObject* tson_array_get_object(const TsonArray* arr, size_t index);

#endif // TSON_LIBRARY_H
