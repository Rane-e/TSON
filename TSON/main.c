#include <stdio.h>
#include "tson_library.h" // Подключение гипотетической библиотеки

int main() {
    // Создание объекта TSON
    TsonObject* tsonObj = tson_object_create();

    // Добавление значений в объект
    tson_object_set_string(tsonObj, "name", "John Doe");
    tson_object_set_integer(tsonObj, "age", 30);
    tson_object_set_boolean(tsonObj, "isStudent", 0);

    TsonArray* gradesArray = tson_array_create();
    tson_array_add_string(gradesArray, "123");
    tson_array_add_float(gradesArray, 88.0f);
    tson_array_add_integer(gradesArray, 35);
    printf("%f\n", tson_array_get_float(gradesArray, 1));
    printf("%d\n", tson_array_get_integer(gradesArray, 2));
    tson_object_set_array(tsonObj, "grades", gradesArray);

    TsonObject* addressObj = tson_object_create();
    tson_object_set_string(addressObj, "street", "123 Main St");
    tson_object_set_string(addressObj, "city", "Anytown");

    tson_object_set_object(tsonObj, "address", addressObj);
    //
    // Сериализация объекта в байтовый массив
    size_t serializedSize;
    unsigned char* serializedData = tson_object_serialize(tsonObj, &serializedSize);
    printf("Serialized Data:\n");
    for (size_t i = 0; i < serializedSize; i++) {
        printf("%02X ", serializedData[i]);
    }
    printf("\n");
    TsonArray* arry = tson_object_get_array(tsonObj, "grades");
    printf("%f\n", tson_array_get_float(arry, 1));
    printf("%d\n", tson_array_get_integer(arry, 2));

    // Десериализация байтового массива обратно в объект
    TsonObject* deserializedObj = tson_object_deserialize(serializedData, serializedSize);

    // Использование десериализованного объекта
    const char* deserializedName = tson_object_get_string(deserializedObj, "name");
    int deserializedAge = tson_object_get_integer(deserializedObj, "age");
    TsonArray* arr = tson_object_get_array(deserializedObj, "grades");


    printf("%s\n", deserializedName);
    printf("%d\n", deserializedAge);
    printf("%f\n", tson_array_get_float(arr, 1));
    printf("%d\n", tson_array_get_integer(arr, 2));

    // Освобождение ресурсов
    tson_object_destroy(tsonObj);
    tson_object_destroy(deserializedObj);
    free(serializedData);

    return 0;
}