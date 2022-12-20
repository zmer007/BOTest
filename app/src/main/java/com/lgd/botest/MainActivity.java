package com.lgd.botest;

import android.os.Bundle;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.lgd.botest.databinding.ActivityMainBinding;
import com.lgd.simplet.Main;

public class MainActivity extends AppCompatActivity {

    private ActivityMainBinding binding;
    Main mCalc = new Main();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(mCalc.stringFromJNI() + ", " + Main.getDeviceId(this));
    }

}