package com.example.wangxi.jnid;

import java.util.Random;
import java.util.UUID;

/**
 * Created by wangxi on 2017/3/2.
 */

public class JNITest {

    public String key="jason";
    public static int count = 9;

    public native String accessField();

    //访问属性，返回修改之后的属性内容

    public native void accessStaticField();

    public native void accessMethod();
//
    public native void accessStaticMethod();

    //产生指定范围的随机数
    public int genRandomInt(int max){
        System.out.println("genRandomInt 执行了...");
        return new Random().nextInt(max);
    }

    //产生UUID字符串
    public static String getUUID(){
        return UUID.randomUUID().toString();
    }

}
