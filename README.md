# PID for Arduino

:construction: This README is under construction and was not reviewed.

---

## 1 Physical mounting

### 1.1 Pinout

Before start, check the Arduino pinout, and module pinout.

//colocar imagem

// colocar imagem

### 1.2 Initial mounting

1. With arduino and PSU off.

2. Connect PSU VCC in VM of L298N.

3. Connect PSU GND to GND of L298N.

4. Connect 5V from L298N to Arduino Vin (optional).

>:warning:**WARNING:** When uploading the code, you must disconnect 5V from 298N and Arduino Vin.

5. Connect 5V from Arduino to one pin of 10K resistor.

6. Connect the another pin of 10K resistor to one pin of thermistor.

7. Connect the another pin of thermistor tho Arduino GND.

8. Now consider the following connections:

|Arduino||L298N|
|----|----|----|
|Digital 6|:left_right_arrow:|EN1|
|Digital 7|:left_right_arrow:|IN1|
|Digital 8|:left_right_arrow:|IN2|

##### 1.2.1 Peltier mode

After done the 1.2:

1. Connect one wire of peltier to OUT1 from L298N.

2. Connect the another wire of peltier to OUT2 from L298N.

3. Set up the PSU to 2A of current, and a voltage between 5V and 6V.

4. Turn on the channel of the PSU.

Now the system is working (if you have uploaded the code to Arduino).


>:warning:**TIP:** If the peltier is getting cold in the hot side, and hot in the cold size. You must invert the peltier wire in OUT1 and OUT2

#### 1.2.2 Resistor mode


After done the step 1.2

1. Connect one wire of resistor to OUT1 of L298N 

2. Connect the another wire of the resistor to OUT2 from L298N

##### 1.2.2.1 One resistor mode

In this case the circuit is ready.

3. Set up the PSU to 2A of current, and a voltage between 10V and 11V.

4. Turn on the channel of the PSU.

Now the system is working (if you have uploaded the code to Arduino).

##### 1.2.2.2 Two resistor mode

Afterr done the 1.2 and 1.2.2 steps

1. Consider the following connections:

|Arduino||L298N|
|----|----|----|
|Digital 5|:left_right_arrow:|EN2|
|Digital 3|:left_right_arrow:|IN3|
|Digital 4|:left_right_arrow:|IN4|

In the another Resistor

2. Connect one wire of the resistor to OUT4 of L298N.

3. Connect the another wire of the resistor to OUT3 of L298N.

4. Set up the PSU to 2A of current, and a voltage between 10V and 11V.

5. Turn on the channel of the PSU.

Now the system is working (if you have uploaded the code to Arduino)

### 1.3 Mounting with resistors and TIP122


1. With arduino and PSU off.

2. Connect PSU VCC in Arduino VCC.

3. Connect on wire of each Resistor in PSU VCC.

4. Connect the remaining Wire of each resistor to TIP122 Collector.

5. Connect pin 6 of Arduino to a resistor with value between 100ohm and 1000ohm.

6. connect the remaining pin of the resistor to TIP122 base.

7. Turn on the Arduino and PSU.

Now the system is working (if you have uploaded the code to Arduino).

## 2. Uploading the code

Select the code between tho options bellow, and open in Arduino IDE.

|Project||Folder|
|----|----|----|
|Peltier|:left_right_arrow:|PID_para_peltier|
|Digital 3|:left_right_arrow:|PID_para_resistores|

1. Disconnect the Arduino to the PC.

2. Go to Tools :arrow_right: Boards :arrow_right: Arduino Avr Boards :arrow_right: Arduino Uno

3. Go to Tools :arrow_right: Port

4. Connect the Arduino to the PC.

5. Select the new port, that will appear.

6. Click in a Symbol like this ":arrow_right:" in the upper left.

The code upload will be done.

## 3. Calibrating the thermistor

1. With a multimeter, get the resistence of 10K resistor, and change in the code the line ``RESISTOR_REF``

2. With the known temperature $T_0$ in Kelvin, change in the code ``T_ZERO``

3. With a multimeter, get the resistence of thermistor, and change in the code the line ``R_ZERO``

4. With another known temperature $T$ in Kelvin such that $T\neq T_0$, get the resistence of thermistor $R$

5. Now consider the following equation $\beta=\frac {\ln(R)-\ln(R_0)}{\frac{1}{T}-\frac{1}{T_0}}$

6. Change in the code the line ``BETA`` with the value of $\beta$

## 4. Reading and saving the data from the experiment

1. Open ``codigo_para_ler.py`` in a Python compiler/IDE.

>:warning:**WARNING:** Do not use Jupyter or colab, both will not interact with arduino. Ando the code will not work.

2. If necessary install the following libraries/modules

>``pip install pyserial``

>``pip install matplotlib``

>``pip install numpy``

>``pip install json``

3. In ``arduino = serial.Serial(port='COM6', baudrate=baudrate)`` change ``COM6`` to the Arduino port

4. In ``exp_time = 1000.0`` change the time, to the experiment time.

5. Run the code.

6. After hitting the experiment time, the graphic will be plotted.

7. if you want to save the result in JSON insert "s" or "s", otherwise, insert "n" or "N".

8. if you chose to save you file, you can name the file, inserting a name.

# 5. Tunning the PID parameters

## 5.1 Standard method

Start with $K_p=K_d=K_i=0$

### 5.1.1 Tunning the $K_p$

1. Increment the $K_p$

2. If the system does not react, and does not correct: go to step 5.1.1.1

3. If the system overreact, and the error does not stabilize, reduce the $K_p$

The ideal $K_p$ is reached, when the system almost correct the error, but is getting asymptotic closer tho zero.

### 5.1.2 Tunning the $K_i$

1. Increment the $K_i$

2. If the still  almost correcting the error, but is getting asymptotic closer tho zero. return to step 5.1.2.1.

3. If the system overreact, and the error does not stabilize, reduce the $K_i$

The ideal $K_i$ is reached when the erros oscilates closer to zero, but the amplitude of error is getting lower with time.

### 5.1.3 Tunning the $K_d$

1. Increase the $K_d$

2. Repeat the step 2, until the error oscilate, now reduce the $K_d$

### 5.2 Ziegler-Nichols Method

Start with $K_p=K_d=K_i=0$

1. Increase the $K_p$ until the error never stabilize, and oscilates. This is $K_n$

2. Measure the time peak-peak, this is $T_n$

Now set the paramters, consdering the system that you will use.


|System|$K_p$|$K_i$|K_d|
|----|----|----|----|
|P|$\frac{K_n}{2}$|0|0|
|PI|$0.45K_n$|$0.54\frac{K_n}{T_n}$|0|
|PID|$0.6K_n$|$1.2\frac{K_n}{T_n}$|$0.075K_nT_n$|



