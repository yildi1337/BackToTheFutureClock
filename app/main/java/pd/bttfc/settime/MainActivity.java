package pd.bttfc.settime;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothSocket;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;
import java.io.IOException;
import java.io.OutputStream;
import java.sql.Timestamp;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.Random;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    public Button buttonSetTime;
    public Button buttonSetRandomTime;
    public Button buttonExit;

    public BluetoothManager btManager;
    public BluetoothAdapter btAdapter;
    public BluetoothDevice btDevice;
    public BluetoothSocket btSocket;

    public static final String UUID_SERIAL_PORT = "00001101-0000-1000-8000-00805f9b34fb";
    public static final String UUID_BTTFC = "00:14:03:11:03:02";

    public static final String START_DATE = "2000-01-01 00:00:00";
    public static final String STOP_DATE = "2099-12-31 23:59:59";

    @SuppressLint({"MissingPermission", "SimpleDateFormat"})
    public void sendDateStringViaBluetooth(Date date) {
        try {
            if (btSocket == null) {
                btManager = (BluetoothManager) getApplicationContext().getSystemService(getApplicationContext().BLUETOOTH_SERVICE);
                btAdapter = btManager.getAdapter();
                btDevice = btAdapter.getRemoteDevice(UUID_BTTFC);
                btSocket = btDevice.createInsecureRfcommSocketToServiceRecord(UUID.fromString(UUID_SERIAL_PORT));
                btSocket.connect();
            }

            if (btSocket != null) {
                try{
                    DateFormat dateFormat;
                    dateFormat = new SimpleDateFormat("yyyy-MM-dd_HH:mm:ss");
                    String strDate = dateFormat.format(date);

                    Toast.makeText(getApplicationContext(), "Writing " + strDate, Toast.LENGTH_SHORT).show();

                    OutputStream out = btSocket.getOutputStream();
                    out.write(strDate.getBytes());

                } catch(IOException e) {
                    Toast.makeText(getApplicationContext(), "IOException: " + e, Toast.LENGTH_SHORT).show();
                    btSocket.close();
                    finish();
                }
            }
        }
        catch (IOException e) {
            Toast.makeText(getApplicationContext(), "IOException: " + e, Toast.LENGTH_SHORT).show();
            try {
                btSocket.close();
            } catch (IOException ex) {
                Toast.makeText(getApplicationContext(), "IOException: " + e, Toast.LENGTH_SHORT).show();
            }
            finish();
        }
    }

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Toast.makeText(getApplicationContext(), "Bonjour!", Toast.LENGTH_SHORT).show();

        buttonSetTime = findViewById(R.id.button_set_time);
        buttonSetRandomTime = findViewById(R.id.button_set_random_time);
        buttonExit = findViewById(R.id.button_exit);

        if (ContextCompat.checkSelfPermission(
                this, android.Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED
        ) {
            ActivityCompat.requestPermissions(this, new String[] { android.Manifest.permission.BLUETOOTH_CONNECT }, 1);
        }

        if (ContextCompat.checkSelfPermission(
                this, android.Manifest.permission.BLUETOOTH_SCAN) != PackageManager.PERMISSION_GRANTED
        ) {
            ActivityCompat.requestPermissions(this, new String[] { android.Manifest.permission.BLUETOOTH_SCAN }, 1);
        }

        buttonSetTime.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Date date = Calendar.getInstance().getTime();
                sendDateStringViaBluetooth(date);
            }
        });

        buttonSetRandomTime.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {

                long startdate = Timestamp.valueOf(START_DATE).getTime();
                long enddate = Timestamp.valueOf(STOP_DATE).getTime();
                Random r = new Random();
                Timestamp timestamp = new Timestamp(startdate + (long) (r.nextFloat() * (enddate - startdate)));
                Date date = new Date(timestamp.getTime());
                sendDateStringViaBluetooth(date);
            }
        });

        buttonExit.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Toast.makeText(getApplicationContext(), "Au revoir!", Toast.LENGTH_SHORT).show();
                try {
                    btSocket.close();
                } catch (IOException e) {
                    Toast.makeText(getApplicationContext(), "IOException: " + e, Toast.LENGTH_SHORT).show();
                }
                finish();
            }
        });
    }
}