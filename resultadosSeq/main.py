import csv
import matplotlib.pyplot as plt
import numpy as np

arquivo = "master_tickers_tempos_seq.csv"

tempos = []
tamanhos = []

with open(arquivo, "r") as f:
    reader = csv.reader(f, delimiter=';')
    for row in reader:
        if len(row) < 6:
            continue
        
        # Coluna 4 posição 3 → tempo
        tempo = float(row[3])

        # Últimas 2 colunas (len_r + len_c)
        len_r = float(row[4])
        len_c = float(row[5])
        soma_tamanhos = len_r + len_c

        tempos.append(tempo)
        tamanhos.append(soma_tamanhos)

# Cálculo estatístico
tempos = np.array(tempos)
media = tempos.mean()
desvio = tempos.std()

print(f"Média dos tempos: {media}")
print(f"Desvio padrão: {desvio}")

# Gráfico de dispersão
plt.figure(figsize=(8,6))
plt.scatter(tamanhos, tempos, alpha=0.7)
plt.xlabel("Soma dos tamanhos (len_r + len_c)")
plt.ylabel("Tempo (coluna 4)")
plt.title("Dispersão: Tempo vs Soma dos Tamanhos")
plt.grid(True)
plt.tight_layout()
plt.show()


# master_tickers_t br us
#Média dos tempos: 62.457797277365465
#Desvio padrão: 12.976022120095696