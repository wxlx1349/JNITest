package com.example.wangxi.jnid;

import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.TextView;

import java.io.File;
import java.util.Date;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    JNITest jniTest;
    String path = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());


        jniTest = new JNITest();
        Log.e("tag", "key=" + jniTest.accessField());
        Log.e("tag", "old count=" + JNITest.count);
        jniTest.accessStaticField();
        Log.e("tag", " count=" + JNITest.count);
        jniTest.accessMethod();
        Log.e("tag", " count1=" + JNITest.count);
        jniTest.accessStaticMethod(path);

        Date date = (Date) jniTest.accessConstructor();
        Log.e("tag", "time=" + date.getTime());
        jniTest.accessNonvirtualMethod();

//        Log.e("tag", "chineseChars=" + jniTest.chineseChars());

        int[] arr = new int[]{7, 4, 9, 0, 3, 6};
        jniTest.giveArray(arr);
        printArr(arr);
        printArr(jniTest.getArray(7));

        jniTest.localRef();

        System.out.println("------08-17------");
        jniTest.createGlobalRef();
        System.out.println(jniTest.getGlobalRef());
        //用完之后释放
        jniTest.deleteGlobalRef();
        System.out.println("释放完了...");
        //System.out.println(t.getGlobalRef());

        try {
            jniTest.exception();
        } catch (Exception e) { //用throwable可以扑捉jni的错误
            System.out.println("发生异常：" + e.getMessage());
        }

        //不断调用cached方法
        for (int i = 0; i < 100; i++) {
            jniTest.cached();
        }

        System.out.println("--------异常发生之后-------");
    }

    private void printArr(int[] arr) {
        for (int i : arr) {
            Log.e("tag", "arr i=" + i);
        }
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}
