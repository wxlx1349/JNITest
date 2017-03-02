package com.example.wangxi.jnid;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    JNITest jniTest;

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
        jniTest.accessStaticMethod();
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}
