#include <jni.h>
#include <string>
#include "com_example_wangxi_jnid_JNITest.h"
#include <iostream>

using namespace std;

extern "C"
jstring
Java_com_example_wangxi_jnid_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

JNIEXPORT jstring JNICALL Java_com_example_wangxi_jnid_JNITest_accessField
        (JNIEnv *env, jobject jobj) {

    jclass cls = env->GetObjectClass(jobj);

    jfieldID fid = env->GetFieldID(cls, "key", "Ljava/lang/String;");

    jstring jstr = (jstring) env->GetObjectField(jobj, fid);
    cout << "key=" << jstr << endl;

    char *c_str = (char *) env->GetStringUTFChars(jstr, JNI_FALSE);
    char text[20] = "super";

    strcat(text, c_str);

    jstring new_jstr = env->NewStringUTF(text);

    env->SetObjectField(jobj, fid, new_jstr);
    return new_jstr;

}

JNIEXPORT void JNICALL Java_com_example_wangxi_jnid_JNITest_accessStaticField
        (JNIEnv *env, jobject jobj) {
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid = env->GetStaticFieldID(cls, "count", "I");
    jint count = env->GetStaticIntField(cls, fid);
    count++;
    env->SetStaticIntField(cls, fid, count);
};

JNIEXPORT void JNICALL Java_com_example_wangxi_jnid_JNITest_accessMethod
        (JNIEnv *env, jobject jobj) {
    jclass cls = env->GetObjectClass(jobj);
    jmethodID mid = env->GetMethodID(cls, "genRandomInt", "(I)I");
    jint random = env->CallIntMethod(jobj, mid, 200);
    printf("random num:%d", random);

    jfieldID fid = env->GetStaticFieldID(cls, "count", "I");
    env->SetStaticIntField(cls, fid, random);

};

JNIEXPORT void JNICALL Java_com_example_wangxi_jnid_JNITest_accessStaticMethod
        (JNIEnv *env, jobject jobj) {
    jclass cls = env->GetObjectClass(jobj);
    jmethodID mid = env->GetStaticMethodID(cls, "getUUID", "()Ljava/lang/String;");
    jstring uuid = (jstring) env->CallStaticObjectMethod(cls, mid);

    char *uuid_str = (char *) env->GetStringUTFChars(uuid, NULL);
    char filename[100];
    sprintf(filename, "D://%s.txt", uuid_str);
    FILE *fp = fopen(filename, "w");
    fputs("i love xiaomei", fp);
    fclose(fp);

};
