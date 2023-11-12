package com.example.pedalp1;

import androidx.appcompat.app.AppCompatActivity;

import android.content.res.AssetManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Spinner;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    private Spinner Lista;
    public static AssetManager assetManager;
    public static File DirectorioArchivos;
    private EnvioArduino Envio = new EnvioArduino();
    private String ESP32IP = "192.168.1.76";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Lista = findViewById(R.id.spnPreinstalados);
        Button BotonEnviar = findViewById(R.id.btnEnviar);
        String[] Efectos = {"Clean", "Distortion", "Delay"};

        ArrayAdapter<String> AdaptadorEfecto = new ArrayAdapter<>(this, android.R.layout.simple_spinner_dropdown_item, Efectos);
        Lista.setAdapter(AdaptadorEfecto);
        assetManager = getAssets();
        DirectorioArchivos = getFilesDir();

        BotonEnviar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                // Inicia la tarea en segundo plano cuando se hace clic en el botón
                String efectoSeleccionado = Lista.getSelectedItem().toString();
                new EnviarArchivoTask().execute(ESP32IP, efectoSeleccionado);
            }
        });
    }

    private static class EnviarArchivoTask extends AsyncTask<String, Void, Void> {
        @Override
        protected Void doInBackground(String... params) {
            String IPESP32 = params[0];
            String EfectoSeleccionado = params[1];

            // Modificar para incluir todos los archivos necesarios para el efecto seleccionado
            List<String> archivos = new ArrayList<>();

            // Agregar archivos según el efecto seleccionado
            if (EfectoSeleccionado.equals("Clean")) {
                archivos.add("PedalClean.ino");
                archivos.add("AudioDspDriver.h");
                archivos.add("AudioDspDriver.cpp");
            } else if (EfectoSeleccionado.equals("Distortion")) {
                archivos.add("PedalDis.ino");
                archivos.add("AudioDspDriver.h");
                archivos.add("AudioDspDriver.cpp");
            } else if (EfectoSeleccionado.equals("Delay")) {
                archivos.add("PedalDe.ino");
                archivos.add("AudioDspDriver.cpp");
                archivos.add("AudioDspDriver.h");
            }

            EnvioArduino envio = new EnvioArduino();
            envio.EnviarArchivo(IPESP32, archivos);
            return null;
        }
    }
}
