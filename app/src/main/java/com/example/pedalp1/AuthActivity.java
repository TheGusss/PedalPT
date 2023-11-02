package com.example.pedalp1;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import com.google.firebase.analytics.FirebaseAnalytics;
import com.google.firebase.auth.FirebaseAuth;

public class AuthActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        try {
            Thread.sleep(500); // Retardo de splash (No recomendado)
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        setTheme(R.style.AppTheme); // Se utiliza splash al momento de cargar la app y Apptheme se aplica cuando carga
        setContentView(R.layout.activity_auth);

        // Analytics Event
        FirebaseAnalytics analytics = FirebaseAnalytics.getInstance(this);
        Bundle bundle = new Bundle();
        bundle.putString("Mesagge", "Integracion de Firebase Completada");
        analytics.logEvent("InitScreen", bundle);

        // Setup
        setup(); // Llama a la función setup
    }

    // Función que se encarga de configurar la lógica
    // se incluye la inicialización de los botones y la lógica para el registro de usuarios.
    private void setup() {
        Button signupButton = findViewById(R.id.saveButton);
        final EditText editEmail = findViewById(R.id.editTextTextEmail2);
        final EditText editPass = findViewById(R.id.editTextTextPassword2);

        signupButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (!editEmail.getText().toString().isEmpty() && !editPass.getText().toString().isEmpty()) {
                    FirebaseAuth.getInstance().createUserWithEmailAndPassword(editEmail.getText().toString(), editPass.getText().toString())
                            .addOnCompleteListener(AuthActivity.this, task -> {
                                if (task.isSuccessful()) {
                                    limpiaraut();
                                    showhome(task.getResult().getUser().getEmail());

                                } else {
                                    showAlert();
                                }
                            });
                }
            }
        });
    }

    // Función error
    private void showAlert() {
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("Error");
        builder.setMessage("Se ha producido un error autenticando al usuario");
        builder.setPositiveButton("Aceptar", null);
        AlertDialog dialog = builder.create();
        dialog.show();
    }

    private void showhome(String email) {
        Intent homeIntent = new Intent(this, MainActivity.class);
        homeIntent.putExtra("email", email);
        startActivity(homeIntent);
    }

    private void limpiaraut() {
        EditText useremail = findViewById(R.id.editTextTextEmail2);
        EditText userpass = findViewById(R.id.editTextTextPassword2);
        useremail.setText("");
        userpass.setText("");
    }

    public void irANuevaVista(View view) {
        Intent intent = new Intent(this, AccessActivity.class);
        startActivity(intent);
    }
}