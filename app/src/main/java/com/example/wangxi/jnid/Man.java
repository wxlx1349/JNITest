package com.example.wangxi.jnid;

/**
 * Created by wangxi on 2017/3/3.
 * <p>
 * description :
 * version code: 1
 * create time : 2017/3/3
 * update time : 2017/3/3
 * last modify : wangxi
 */

public class Man extends Human{
    @Override
    public void sayHi() {
        System.out.println("男人打招呼，儿子是我的...");
    }

    public void sayHi2(){
        super.sayHi();
    }

}
