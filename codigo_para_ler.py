import serial, json, re
import matplotlib.pyplot as plt
import time


baudrate = 9600
arduino = serial.Serial(port='COM8', baudrate=baudrate) #troque COM6 pela sua porta

use = [1, 1, 1, 1, 1, 1]
exp_time = 900.0 #defina o tempo de seu experimento em segundos

elapsed_time_exp = time.time()
list_tempo, list_errtemp = [], []
list_integral_err,list_power_err=[],[]
print("coletando dados, por favor aguarde")
version = "v1.1.1"
time_v = time.time()



while time.time() - elapsed_time_exp < exp_time:
    

    text = str(arduino.readline())
    text = text.split(" ")
    arduino_els_time = re.sub('[^0-9]', '', text[0])
    error_temp =int(re.sub('[^0-9]', '', text[1]))/100
    integral_err=int(re.sub('[^0-9]', '', text[2]))/100
    power_err=int(re.sub('[^0-9]', '', text[3]))
    tmpkkk = int(arduino_els_time)
 
    list_tempo.append(tmpkkk/1000)
    list_errtemp.append(error_temp)
    list_integral_err.append(integral_err-10000.0)
    list_power_err.append(power_err)
    

time = time.time() - time_v
arduino.close()

plt.title("Temperatura ao longo do tempo")
plt.plot(list_tempo, list_errtemp, '-', label='Erro')
plt.axhline(y=34.5, color='r', linestyle='-')
plt.xlabel("tempo[ms]")
plt.ylabel("erro [°C]", color='red')
plt.legend()
plt.show()

plt.title("Integral de erro ao longo do tempo")
plt.plot(list_tempo, list_integral_err, '-', label='Integral')
plt.xlabel("tempo[ms]")
plt.ylabel("Integral[°C*s]", color='red')
plt.legend()
plt.show()

plt.title("Potencia aplicada em PWM")
plt.plot(list_tempo, list_power_err, '-', label='Potencia')
plt.xlabel("tempo[ms]")
plt.ylabel("PWM", color='red')
plt.legend()
plt.show()



if len(list_tempo) > 67:
    print(f'o sample rate foi de {1000/(list_tempo[67] - list_tempo[66])} Hz')

meta = [baudrate, time, version]
savedict = {'tempo': list_tempo, 'erro': list_errtemp,'integral':list_integral_err,'potencia':list_power_err, 'metadados': meta}
json_object = json.dumps(savedict, indent=1)

if input("salvar? ") not in ["N", "n"]:
    name = input("insira o nome do experimento: ")
    filename = f"exp p5 {name}.json" if name else f"exp {time.time()}.json"
    with open(filename, "w") as outfile:
        outfile.write(json_object)
