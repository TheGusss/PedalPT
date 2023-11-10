package com.example.pedalp1;
import android.content.res.AssetManager;

import  org.apache.commons.net.ftp.FTP;
import  org.apache.commons.net.ftp.FTPClient;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.SocketException;

public class EnvioArduino {
    public void EnviarArchivo(String IPESP32, String NombreArchivo){
        FTPClient clienteftp = new FTPClient();
        try {
            /* Configura la conexion y envio FTP */
            clienteftp.connect(IPESP32);
            clienteftp.login("esp32prueba", "12345678");
            clienteftp.enterLocalPassiveMode();
            clienteftp.setFileType(FTP.BINARY_FILE_TYPE);

            /* preparando el Archivo */
            AssetManager assetManager = MainActivity.assetManager;
            InputStream inputStream = assetManager.open(NombreArchivo + ".ino"); //Encuentra el archivo en Assets

            // Crea un archivo temporal para copiar el contenido
            File outputFile = new File(MainActivity.DirectorioArchivos, "temp_file");

            // Copia el contenido del archivo de "Assets" al archivo temporal
            FileOutputStream outputStream = new FileOutputStream(outputFile);
            byte[] buffer = new byte[1024];
            int length;
            while ((length = inputStream.read(buffer)) > 0) {
                outputStream.write(buffer, 0, length);
            }

            FileInputStream ArchivoLocal = new FileInputStream(outputFile); //recoge el archivo .ino para enviarlo

            /* Se realiza el envio del archivo */
            if (clienteftp.storeFile("/upload"+ NombreArchivo, ArchivoLocal))
            {
                System.out.println("Archivo cargado exitosamente en el ESP32.");
            } else {
                System.out.println("No se pudo cargar el archivo en el ESP32.");
            }

            /* Se cierran las conexiones para mas seguridad*/
            outputStream.close();
            inputStream.close();
            ArchivoLocal.close();
            clienteftp.logout();
        }
        /*Gestion de errores*/
        catch (IOException e) {
            throw new RuntimeException(e);
        }
        finally { //Si aun no se a cerrado la conexion se vuelve a mandar la solicitud de cerrar la sesion FTP
            try {
                if (clienteftp.isConnected()){
                    clienteftp.logout();
                }
            }
            catch (IOException e){
                e.printStackTrace();
            }
        }
    }
}
