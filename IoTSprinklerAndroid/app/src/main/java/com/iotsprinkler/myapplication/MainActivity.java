package com.iotsprinkler.myapplication;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.ValueEventListener;

public class MainActivity extends AppCompatActivity {

    private TextView humidityTextView;
    private TextView timestamp_textview;
    private ProgressBar humidity_progress;
    private Button setThresholdButton;
    private EditText thresholdEditText;
    private Button readingStart;
    private Button pumpStart;
    private ImageView gifImageView;
    private DatabaseReference mDatabase;
    private FirebaseDatabase database;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        humidityTextView = findViewById(R.id.humidity_value_textview);
        timestamp_textview = findViewById(R.id.last_timestamp_textview);
        humidity_progress = findViewById(R.id.humidity_progress_bar);
        setThresholdButton = findViewById(R.id.set_threshold_button);
        thresholdEditText = findViewById(R.id.threshold_edittext);
        readingStart = findViewById(R.id.read_sensor_button);
        pumpStart = findViewById(R.id.watering_button);
        gifImageView = findViewById(R.id.gif_image);

        database = FirebaseDatabase.getInstance();
        mDatabase = FirebaseDatabase.getInstance().getReference();
        mDatabase.child("sensorData").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                int humidityValue = dataSnapshot.child("humidity").getValue(Integer.class);
                int threshold = dataSnapshot.child("threshold").getValue(Integer.class);
                humidityTextView.setText(" " + humidityValue);

                humidity_progress.setProgress(humidityValue);

                if (humidityValue < threshold) {
                    gifImageView.setImageResource(R.drawable.help);
                } else {
                    gifImageView.setImageResource(R.drawable.watered);
                }
            }

            @Override
            public void onCancelled(DatabaseError databaseError) {
                // Handle error
            }
        });

        mDatabase.child("sensorData").child("timeStamp").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                String timestamp = dataSnapshot.getValue(String.class);
                timestamp_textview.setText(" " + timestamp);
            }

            @Override
            public void onCancelled(DatabaseError databaseError) {
                // Handle error
            }
        });

        setThresholdButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String thresholdValue = thresholdEditText.getText().toString().trim();
                if (!thresholdValue.isEmpty()) {
                    int threshold = Integer.parseInt(thresholdValue);
                    database.getReference("sensorData/threshold").setValue(threshold);
                }
            }
        });

        readingStart.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                database.getReference("sensorData/readingStatus").setValue(true);
            }
        });

        pumpStart.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                database.getReference("sensorData/pumpStatus").setValue(true);
            }
        });
    }
}


