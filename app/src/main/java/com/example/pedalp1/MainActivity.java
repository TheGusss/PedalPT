package com.example.pedalp1;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.Spinner;

public class MainActivity extends AppCompatActivity {
    private Spinner Lista;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Lista = (Spinner) findViewById(R.id.spnPreinstalados);
        String [] Efectos = {"Clean", "Distortion", "Boost"};
        ArrayAdapter <String> AdaptadorEfecto = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_dropdown_item, Efectos);
        Lista.setAdapter(AdaptadorEfecto);

    }
}