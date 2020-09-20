package com.avtograv.smartavtograv;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.CompoundButton;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;

import androidx.appcompat.app.AppCompatActivity;
import androidx.constraintlayout.widget.Group;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Set;
import java.util.UUID;


import static android.R.layout.simple_list_item_1;

public class MainActivity extends AppCompatActivity implements CompoundButton.OnCheckedChangeListener {

    ToggleButton tb1, tb2, tb3, tb4;
    TextView textTemp;
    Group groupToggleButton;
    private static final int REQUEST_ENABLE_BT = 1;
    public TextView textInfo;
    BluetoothAdapter bluetoothAdapter;
    ArrayList<String> pairedDeviceArrayList;
    ListView listViewPairedDevice;
    ArrayAdapter<String> pairedDeviceAdapter;
    ThreadConnectBTdevice myThreadConnectBTdevice;
    ThreadConnected myThreadConnected;
    private UUID myUUID;
    private StringBuilder sb = new StringBuilder();


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        textTemp = findViewById(R.id.textTemperature);

        tb1 = findViewById(R.id.toggle_button_1);
        tb2 = findViewById(R.id.toggle_button_2);
        tb3 = findViewById(R.id.toggle_button_3);
        tb4 = findViewById(R.id.toggle_button_4);

        tb1.setOnCheckedChangeListener(this);
        tb2.setOnCheckedChangeListener(this);
        tb3.setOnCheckedChangeListener(this);
        tb4.setOnCheckedChangeListener(this);

        final String UUID_STRING_WELL_KNOWN_SPP = "00001101-0000-1000-8000-00805F9B34FB";
        textInfo = findViewById(R.id.textInfo);
        listViewPairedDevice = findViewById(R.id.pair_id_list);

        groupToggleButton = findViewById(R.id.group_toggle_button);


        if (!getPackageManager().hasSystemFeature(PackageManager.FEATURE_BLUETOOTH)) {
            Toast.makeText(this, "BLUETOOTH NOT support", Toast.LENGTH_LONG).show();
            finish();
            return;
        }
        myUUID = UUID.fromString(UUID_STRING_WELL_KNOWN_SPP);
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (bluetoothAdapter == null) {
            Toast.makeText(this, "Bluetooth is not supported on this hardware platform", Toast.LENGTH_LONG).show();
            finish();
            return;
        }
        @SuppressLint("HardwareIds") String stInfo = bluetoothAdapter.getName() + " : " + bluetoothAdapter.getAddress();
        textInfo.setText(String.format("Имя и IMEI вашего устройства:\n%s", stInfo));
    }

    // Запрос на включение Bluetooth
    @Override
    protected void onStart() {
        super.onStart();
        if (!bluetoothAdapter.isEnabled()) {
            Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableIntent, REQUEST_ENABLE_BT);
        }
        setup();
    }


    // Создание списка сопряжённых Bluetooth-устройств
    private void setup() {
        Set<BluetoothDevice> pairedDevices = bluetoothAdapter.getBondedDevices();

        // Если есть сопряжённые устройства
        if (pairedDevices.size() > 0) {
            pairedDeviceArrayList = new ArrayList<>();

            // Добавляем сопряжённые устройства - Имя + MAC-адресс
            for (BluetoothDevice device : pairedDevices) {
                pairedDeviceArrayList.add(device.getName() + "\n" + device.getAddress());
            }
            pairedDeviceAdapter = new ArrayAdapter<>(this, simple_list_item_1, pairedDeviceArrayList);
            listViewPairedDevice.setAdapter(pairedDeviceAdapter);
            listViewPairedDevice.setOnItemClickListener(new AdapterView.OnItemClickListener() { // Клик по нужному устройству

                @Override
                public void onItemClick(AdapterView<?> parent, View view, int position, long id) { //тут пробел после скобки !!!!

                    // После клика скрываем список
                    listViewPairedDevice.setVisibility(View.GONE);
                    String itemValue = (String) listViewPairedDevice.getItemAtPosition(position);

                    // Вычленяем MAC-адрес
                    String MAC = itemValue.substring(itemValue.length() - 17);
                    BluetoothDevice device2 = bluetoothAdapter.getRemoteDevice(MAC);
                    myThreadConnectBTdevice = new ThreadConnectBTdevice(device2);

                    // Запускаем поток для подключения Bluetooth
                    myThreadConnectBTdevice.start();
                }
            });
        }
    }

    // Закрытие приложения
    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (myThreadConnectBTdevice != null) myThreadConnectBTdevice.cancel();
    }


    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        // Если разрешили включить Bluetooth, тогда void setup()
        if (requestCode == REQUEST_ENABLE_BT) {
            if (resultCode == Activity.RESULT_OK) {
                setup();
                // Если не разрешили, тогда закрываем приложение
            } else {
                Toast.makeText(this, "BlueTooth не включён", Toast.LENGTH_SHORT).show();
                finish();
            }
        }
    }

    // Поток для коннекта с Bluetooth
    private class ThreadConnectBTdevice extends Thread {
        private BluetoothSocket bluetoothSocket = null;

        private ThreadConnectBTdevice(BluetoothDevice device) {
            try {
                bluetoothSocket = device.createRfcommSocketToServiceRecord(myUUID);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        // Коннект
        @Override
        public void run() {
            boolean success = false;
            try {
                bluetoothSocket.connect();
                success = true;
            } catch (IOException e) {
                e.printStackTrace();
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        Toast.makeText(MainActivity.this, "Нет подключения," +
                                " проверьте Bluetooth-устройство с которым хотите соединиться!", Toast.LENGTH_LONG).show();
                        listViewPairedDevice.setVisibility(View.VISIBLE);
                    }
                });
                try {
                    bluetoothSocket.close();
                } catch (IOException e1) {
                    e1.printStackTrace();
                }
            }

            // Если законнектились, тогда открываем панель с кнопками и запускаем поток приёма и отправки данных
            if (success) {
                runOnUiThread(new Runnable() {

                    @SuppressLint("SetTextI18n")
                    @Override
                    public void run() {

                        // открываем панель с кнопками
                        groupToggleButton.setVisibility(View.VISIBLE);
                        textTemp.setText("Измерение" + "\n" + "температуры");

                        if (myThreadConnected != null) {
                            if (textTemp.getText() == "Измерение" + "\n" + "температуры") {

                                // запрос на измерение температуры
                                byte[] bytesToSend = "r".getBytes();
                                myThreadConnected.write(bytesToSend);
                            }
                        }
                    }
                });

                myThreadConnected = new ThreadConnected(bluetoothSocket);

                // запуск потока приёма и отправки данных
                myThreadConnected.start();
            }
        }

        public void cancel() {
            Toast.makeText(getApplicationContext(), "Close - BluetoothSocket", Toast.LENGTH_LONG).show();
            try {
                bluetoothSocket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

    }       // END ThreadConnectBTdevice


    // Поток - приём и отправка данных
    private class ThreadConnected extends Thread {

        // InputStream - абстрактный класс, описывающий поток ввода
        private final InputStream connectedInputStream;
        private final OutputStream connectedOutputStream;
        private String sbprint;

        public ThreadConnected(BluetoothSocket socket) {
            InputStream in = null;
            OutputStream out = null;
            try {
                in = socket.getInputStream();
                out = socket.getOutputStream();
            } catch (IOException e) {
                e.printStackTrace();
            }
            connectedInputStream = in;
            connectedOutputStream = out;
        }


        @Override                           // Приём данных
        public void run() {
            while (true) {
                try {
                    byte[] buffer = new byte[1];

                    // int read(byte[] buffer) - читает байты в буфер,
                    // возвращая количество прочитанных байтов.
                    // По достижении конца файла возвращает значение -1
                    int bytes = connectedInputStream.read(buffer);

                    // задаём диапазон символьного массива
                    // Указываем сам массив байтов, начало диапазона и
                    // количество символов для записи в строку.
                    String strIncom = new String(buffer, 0, bytes);

                    // собираем символы в строку
                    sb.append(strIncom);

                    // определяем конец строки
                    int endOfLineIndex = sb.indexOf("\r\n");

                    if (endOfLineIndex > 0) {
                        sbprint = sb.substring(0, endOfLineIndex);
                        sb.delete(0, sb.length());

                        runOnUiThread(new Runnable() { // Вывод данных

                            @SuppressLint("SetTextI18n")
                            @Override
                            public void run() {
                                if (sbprint.contains("temp")) {
                                    textTemp.setText(sbprint + "\u00B0");
                                } else if (sbprint.contains("request")) {
                                    textTemp.setText(sbprint + "\u00B0");
                                    Toast.makeText(MainActivity.this, "Температура по запросу", Toast.LENGTH_SHORT).show();
                                } else {
                                    switch (sbprint) {

                                        case "iButton":
                                            Toast.makeText(MainActivity.this, sbprint, Toast.LENGTH_SHORT).show();
                                            break;

                                        case "D10 ON":
                                            Toast.makeText(MainActivity.this, sbprint, Toast.LENGTH_SHORT).show();
                                            break;

                                        case "D10 OFF":
                                            Toast.makeText(MainActivity.this, sbprint, Toast.LENGTH_SHORT).show();
                                            break;

                                        case "D11 ON":
                                            Toast.makeText(MainActivity.this, sbprint, Toast.LENGTH_SHORT).show();
                                            break;

                                        case "D11 OFF":
                                            Toast.makeText(MainActivity.this, sbprint, Toast.LENGTH_SHORT).show();
                                            break;

                                        case "D12 ON":
                                            Toast.makeText(MainActivity.this, sbprint, Toast.LENGTH_SHORT).show();
                                            break;

                                        case "D12 OFF":
                                            Toast.makeText(MainActivity.this, sbprint, Toast.LENGTH_SHORT).show();
                                            break;

                                        case "D13 ON":
                                            Toast.makeText(MainActivity.this, sbprint, Toast.LENGTH_SHORT).show();
                                            break;

                                        case "D13 OFF":
                                            Toast.makeText(MainActivity.this, sbprint, Toast.LENGTH_SHORT).show();
                                            break;

                                        default:
                                            break;
                                    }
                                }
                            }
                        });
                    }
                } catch (IOException e) {
                    break;
                }
            }
        }

        public void write(byte[] buffer) {
            try {
                connectedOutputStream.write(buffer);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }


    // отправляем данные
    @Override
    public void onCheckedChanged(CompoundButton compoundButton, boolean isChecked) {
        switch (compoundButton.getId()) {
            case R.id.toggle_button_1:
                if (isChecked) {
                    if (myThreadConnected != null) {
                        byte[] bytesToSend = "a".getBytes();
                        myThreadConnected.write(bytesToSend);
                    }
                    Toast.makeText(MainActivity.this, "D10 ON", Toast.LENGTH_SHORT).show();
                } else {
                    if (myThreadConnected != null) {
                        byte[] bytesToSend = "A".getBytes();
                        myThreadConnected.write(bytesToSend);
                    }
                    Toast.makeText(MainActivity.this, "D10 OFF", Toast.LENGTH_SHORT).show();
                }
                break;
            case R.id.toggle_button_2:
                if (isChecked) {
                    if (myThreadConnected != null) {

                        byte[] bytesToSend = "b".getBytes();
                        myThreadConnected.write(bytesToSend);
                    }

                    Toast.makeText(MainActivity.this, "D11 ON", Toast.LENGTH_SHORT).show();
                } else {
                    if (myThreadConnected != null) {

                        byte[] bytesToSend = "B".getBytes();
                        myThreadConnected.write(bytesToSend);
                    }

                    Toast.makeText(MainActivity.this, "D11 OFF", Toast.LENGTH_SHORT).show();
                }
                break;
            case R.id.toggle_button_3:
                if (isChecked) {
                    if (myThreadConnected != null) {

                        byte[] bytesToSend = "c".getBytes();
                        myThreadConnected.write(bytesToSend);
                    }

                    Toast.makeText(MainActivity.this, "D12 ON", Toast.LENGTH_SHORT).show();
                } else {
                    if (myThreadConnected != null) {

                        byte[] bytesToSend = "C".getBytes();
                        myThreadConnected.write(bytesToSend);
                    }

                    Toast.makeText(MainActivity.this, "D12 OFF", Toast.LENGTH_SHORT).show();
                }
                break;
            case R.id.toggle_button_4:
                if (isChecked) {
                    if (myThreadConnected != null) {

                        byte[] bytesToSend = "d".getBytes();
                        myThreadConnected.write(bytesToSend);
                    }

                    Toast.makeText(MainActivity.this, "D13 ON", Toast.LENGTH_SHORT).show();
                } else {
                    if (myThreadConnected != null) {

                        byte[] bytesToSend = "D".getBytes();
                        myThreadConnected.write(bytesToSend);
                    }

                    Toast.makeText(MainActivity.this, "D13 OFF", Toast.LENGTH_SHORT).show();
                }
                break;
        }
    }
}