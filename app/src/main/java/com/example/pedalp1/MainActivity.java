package com.example.pedalp1;

import androidx.appcompat.app.AppCompatActivity;

import android.content.res.AssetManager;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Spinner;

import java.io.File;

public class MainActivity extends AppCompatActivity {
    private Spinner Lista;
    public static AssetManager assetManager;
    public static File DirectorioArchivos;
    private EnvioArduino Envio = new EnvioArduino();
    private String ESP32IP = "0.0.0.0"; //Encontrar alguna forma de que el ESP32 mande su IP para configurar el envio
    //Para la pruebas solo sustituir el texto con la IP
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Lista = (Spinner) findViewById(R.id.spnPreinstalados);
        Button BotonEnviar = findViewById(R.id.btnEnviar);
        String [] Efectos = {"Clean", "Distortion", "Super Crunch"};


        ArrayAdapter <String> AdaptadorEfecto = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_dropdown_item, Efectos);
        Lista.setAdapter(AdaptadorEfecto);
        assetManager = getAssets();
        DirectorioArchivos = getFilesDir();

        BotonEnviar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Envio.EnviarArchivo(ESP32IP, Lista.getSelectedItem().toString());
            }
        });
    }
}