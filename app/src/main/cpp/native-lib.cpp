#include <jni.h>
#include <string.h>
#include "com_example_wangxi_jnid_JNITest.h"
#include <iostream>
#include <stdlib.h>
#include "log_utils.h"


using namespace std;

extern "C"
jstring
Java_com_example_wangxi_jnid_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

//访问java的field
JNIEXPORT jstring JNICALL Java_com_example_wangxi_jnid_JNITest_accessField
        (JNIEnv *env, jobject jobj) {
//获得class
    jclass cls = env->GetObjectClass(jobj);
//获得字段的id,   属性名称，属性签名
    jfieldID fid = env->GetFieldID(cls, "key", "Ljava/lang/String;");

    jstring jstr = (jstring) env->GetObjectField(jobj, fid);
//    LOGE("key=%s", jstr);
    cout << "key=" << jstr << endl;

    char *c_str = (char *) env->GetStringUTFChars(jstr, JNI_FALSE);
    char text[20] = "super";

    strcat(text, c_str);

    jstring new_jstr = env->NewStringUTF(text);

    env->SetObjectField(jobj, fid, new_jstr);
    return new_jstr;

}

//获得java的静态字段
JNIEXPORT void JNICALL Java_com_example_wangxi_jnid_JNITest_accessStaticField
        (JNIEnv *env, jobject jobj) {
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid = env->GetStaticFieldID(cls, "count", "I");
    jint count = env->GetStaticIntField(cls, fid);
    count++;
    env->SetStaticIntField(cls, fid, count);
};

//获得java的方法
JNIEXPORT void JNICALL Java_com_example_wangxi_jnid_JNITest_accessMethod
        (JNIEnv *env, jobject jobj) {
    jclass cls = env->GetObjectClass(jobj);
    jmethodID mid = env->GetMethodID(cls, "genRandomInt", "(I)I");
    jint random = env->CallIntMethod(jobj, mid, 200);
    printf("random num:%d", random);
//    LOGE("random num=%d", random);

    jfieldID fid = env->GetStaticFieldID(cls, "count", "I");
    env->SetStaticIntField(cls, fid, random);

};

//访问静态方法
JNIEXPORT void JNICALL Java_com_example_wangxi_jnid_JNITest_accessStaticMethod
        (JNIEnv *env, jobject jobj, jstring path) {
    jclass cls = env->GetObjectClass(jobj);
    jmethodID mid = env->GetStaticMethodID(cls, "getUUID", "()Ljava/lang/String;");
    jstring uuid = (jstring) env->CallStaticObjectMethod(cls, mid);

    char *uuid_str = (char *) env->GetStringUTFChars(uuid, NULL);
    char *path_str = (char *) env->GetStringUTFChars(path, NULL);
    char filename[100];
    strcat(filename, path_str);
    strcat(filename, uuid_str);
    strcat(filename, ".txt");
//    sprintf(filename, path_str, uuid_str);
//    LOGE("filename=%s", filename);
//        FILE *fp = fopen(filename, "w");
//        fputs("i love xiaomei", fp);
//        fputs(filename,fp);
//        fclose(fp);

};

//访问构造方法
JNIEXPORT jobject JNICALL Java_com_example_wangxi_jnid_JNITest_accessConstructor
        (JNIEnv *env, jobject jobj) {
    jclass cls = env->FindClass("java/util/Date");
    jmethodID constructor_mid = env->GetMethodID(cls, "<init>", "()V");
    jobject date_obj = env->NewObject(cls, constructor_mid);

    jmethodID mid = env->GetMethodID(cls, "getTime", "()J");
    jlong time = env->CallLongMethod(date_obj, mid);
//    LOGE("time=%lld",time);
    return date_obj;
};

//调用父类的方法
JNIEXPORT void JNICALL Java_com_example_wangxi_jnid_JNITest_accessNonvirtualMethod
        (JNIEnv *env, jobject jobj) {
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid = env->GetFieldID(cls, "human", "Lcom/example/wangxi/jnid/Human;");
    jobject human_obj = env->GetObjectField(jobj, fid);

    jclass human_cls = env->FindClass("com/example/wangxi/jnid/Human");
    jmethodID mid = env->GetMethodID(human_cls, "sayHi", "()V");

//    env->CallVoidMethod(human_obj, mid);
    //调用不覆盖的父类的方法，对应父类是human_clas
    env->CallNonvirtualVoidMethod(human_obj, human_cls, mid);

};

//中文问题
JNIEXPORT jstring JNICALL Java_com_example_wangxi_jnid_JNITest_chineseChars
        (JNIEnv *env, jobject jobj) {
    char *c_str = "马蓉与宋江";

    jclass str_cls = env->FindClass("java/lang/String");
    jmethodID constructor_mid = env->GetMethodID(str_cls, "<init>", "([BLjava/lang/String;)V");

    //jstring jstr = (*env)->NewStringUTF(env, c_str);
    //执行String(byte bytes[], String charsetName)构造方法需要的条件
    //1.jmethodID
    //2.byte数组
    //3.字符编码jstring

    jbyteArray bytes = env->NewByteArray(strlen(c_str));
    //byte数组赋值
    //0->strlen(c_str)，从头到尾
    //对等于，从c_str这个字符数组，复制到bytes这个字符数组
    env->SetByteArrayRegion(bytes, 0, strlen(c_str), (const jbyte *) c_str);

    jstring charsetName = env->NewStringUTF("GB2312");
    return (jstring) env->NewObject(str_cls, constructor_mid, bytes, charsetName);

};

int compare(const int *a, const int *b) {
    return (*a) - (*b);
}

JNIEXPORT void JNICALL Java_com_example_wangxi_jnid_JNITest_giveArray
        (JNIEnv *env, jobject jobj, jintArray arr) {
    //jintArray -> jint指针 -> c int 数组
    jint *elems = env->GetIntArrayElements(arr, NULL);

    //数组的长度
    int len = env->GetArrayLength(arr);
    //排序
    qsort(elems, len, sizeof(jint), (int (*)(const void *, const void *)) compare);
//同步
    //mode
    //0, Java数组进行更新，并且释放C/C++数组
    //JNI_ABORT, Java数组不进行更新，但是释放C/C++数组
    //JNI_COMMIT，Java数组进行更新，不释放C/C++数组（函数执行完，数组还是会释放）

    env->ReleaseIntArrayElements(arr, elems, JNI_COMMIT);
};

JNIEXPORT jintArray JNICALL Java_com_example_wangxi_jnid_JNITest_getArray
        (JNIEnv *env, jobject jobj, jint len) {
    jintArray jint_arr = env->NewIntArray(len);
    jint *elems = env->GetIntArrayElements(jint_arr, NULL);
    int i = 0;
    for (; i < len; i++) {
        elems[i] = i;
    }
    //同步
    env->ReleaseIntArrayElements(jint_arr, elems, JNI_COMMIT);
    return jint_arr;

};

//JNI 引用变量
//引用类型：局部引用和全局引用
//作用：在JNI中告知虚拟机何时回收一个JNI变量

//局部引用，通过DeleteLocalRef手动释放对象
//1.访问一个很大的java对象，使用完之后，还要进行复杂的耗时操作
//2.创建了大量的局部引用，占用了太多的内存，而且这些局部引用跟后面的操作没有关联性

//模拟：循环创建数组
JNIEXPORT void JNICALL Java_com_example_wangxi_jnid_JNITest_localRef
        (JNIEnv *env,jobject jobj){
    int i=0;
    for(;i<5;i++){
        jclass cls=env->FindClass("java/util/Date");
        jmethodID constructor_mid=env->GetMethodID(cls,"<init>","()V");
        jobject obj=env->NewObject(cls,constructor_mid);
        //此处省略一百行代码...

        //不在使用jobject对象了
        //通知垃圾回收器回收这些对象
        env->DeleteLocalRef(obj);
        //此处省略一百行代码...
    }
};

jstring global_str;

JNIEXPORT void JNICALL Java_com_example_wangxi_jnid_JNITest_createGlobalRef
        (JNIEnv *env,jobject jobj){
    jstring obj=env->NewStringUTF("I love xm");
    global_str= (jstring) env->NewGlobalRef(obj);
};

JNIEXPORT jstring JNICALL Java_com_example_wangxi_jnid_JNITest_getGlobalRef
        (JNIEnv *env,jobject jobj){
    return global_str;
};

JNIEXPORT void JNICALL Java_com_example_wangxi_jnid_JNITest_deleteGlobalRef
        (JNIEnv *env,jobject jobj){
    env->DeleteGlobalRef(global_str);
};

//弱全局引用
//节省内存，在内存不足时可以是释放所引用的对象
//可以引用一个不常用的对象，如果为NULL，临时创建
//创建：NewWeakGlobalRef,销毁：DeleteGlobalWeakRef

//异常处理
//1.保证Java代码可以运行
//2.补救措施保证C代码继续运行

//JNI自己抛出的Throwable异常，可以只能在C层清空
//用户通过ThrowNew抛出的异常，可以在Java层捕捉
JNIEXPORT void JNICALL Java_com_example_wangxi_jnid_JNITest_exception
        (JNIEnv *env,jobject jobj){
    jclass cls=env->GetObjectClass(jobj);
    jfieldID fid=env->GetFieldID(cls,"key2","Ljava/lang/String;");
    //检测是否发生Java异常
    jthrowable exception = env->ExceptionOccurred();
    if(exception!=NULL){
        //让Java代码可以继续运行
        //清空异常信息
        env->ExceptionClear();
        fid=env->GetFieldID(cls,"key","Ljava/lang/String;");
    }

    jstring jstr= (jstring) env->GetObjectField(jobj, fid);
    char *str= (char *) env->GetStringUTFChars(jstr, NULL);
//对比属性值是否合法
    if(strcmp(str,"xiao mei")!=0){
//认为抛出异常，给Java层处理
        jclass newExcCls=env->FindClass("java/lang/IllegalArgumentException");
        env->ThrowNew(newExcCls,"key value is invalid!");
    }

};

//局部静态变量只能局部用，但是只初始化一次
JNIEXPORT void JNICALL Java_com_example_wangxi_jnid_JNITest_cached
        (JNIEnv *env,jobject jobj){
    jclass  cls=env->GetObjectClass(jobj);
    //获取jfieldID只获取一次
    //局部静态变量
    static jfieldID key_id = NULL;
    if (key_id == NULL){
        key_id = env->GetFieldID( cls, "key", "Ljava/lang/String;");
        printf("--------GetFieldID-------\n");
    }
};

//初始化全局变量，动态库加载完成之后，立刻缓存起来
jfieldID key_fid;
jmethodID random_mid;
JNIEXPORT void JNICALL Java_com_example_wangxi_jnid_JNITest_initIds(JNIEnv *env, jclass jcls){
    key_fid = env->GetFieldID( jcls, "key", "Ljava/lang/String;");
    random_mid = env->GetMethodID( jcls, "genRandomInt", "(I)I");
}





