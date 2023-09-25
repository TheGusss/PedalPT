package com.example.pedalp1;


import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import com.google.firebase.auth.FirebaseAuth;

public class AccessActivity extends AppCompatActivity {

    // Permite declarar sin asignarle un valor inicial
   Button logingButton;
   EditText editEmail1;
   EditText editPass1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setTheme(R.style.AppTheme); // Establece el tema
        setContentView(R.layout.activity_access);

        // Inicializa las propiedades después de llamar a setContentView
        logingButton = findViewById(R.id.accederbtn);
        editEmail1 = findViewById(R.id.editTextTextEmail);
        editPass1 = findViewById(R.id.editTextTextPassword);

        setup();
    }

    private void setup() {
        logingButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (!editEmail1.getText().toString().isEmpty() && !editPass1.getText().toString().isEmpty()) {
                    FirebaseAuth.getInstance().signInWithEmailAndPassword(
                            editEmail1.getText().toString(),
                            editPass1.getText().toString()
                    ).addOnCompleteListener(task -> {
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

    private void limpiaraut() {
        editEmail1.setText("");
        editPass1.setText("");
    }

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
}