# Simulador de Sistema Operacional

Gerência de processos:
    - PID
    - PCB
        - Contexto de Hardware (não precisa)
        - Contexto de software (prioridade, PID, PPID, Status)
        - Espaço de endereçamento (não precisa)
    - Scheduler (3 filas: fila de alta, fila de baixa, fila de I/O)

Filas de I/O:
    - Disco -> retorna para fila 2
    - Fita -> retorna para fila 1
    - Impressora -> retorna para fila 1

Ordem de entrada na fila de prontos:
    - Novos -> fila 1
    - I/O -> depende do tipo
    - Preemptados -> fila 2

Premissas:
    - Limite de número de processos
    - Fatia de tempo
    - Tempo de serviço (aleatório)
    - Tempo de I/O (aleatório)
        - Disco -> x unidades de tempo (aleatório num intervalo razoável?)
        - Fita -> x unidades de tempo (aleatório num intervalo razoável?)
        - Impressora -> x unidades de tempo (aleatório num intervalo razoável?)
    - Tipo de I/O (aleatório)

Monitoramento:
    - Importante mostrar o estado do simulador no instante de tempo

Relatório:
    - Objetivo
    - Premissas
    - Saída de uma simulação

Apresentação:
    - 20 minutos