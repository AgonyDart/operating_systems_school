import matplotlib.pyplot as plt

class Node:
    def __init__(self, base, free):
        self.base = base
        self.free = free
        self.next = None


class LinkedList:
    def __init__(self):
        self.head = Node(0, 0)

    def add(self, base, free_space):
        new_node = Node(base, free_space)
        self.head.next = new_node

    def show(self):
        actual = self.head.next
        while actual is not None:
            print(f"Start: {actual.base}, Space: {actual.free}")
            actual = actual.next

def show_bar():
    plt.bar(method.keys(), method.values(), width=0.5, align='center')

    plt.ylabel('Memory')
    plt.title(method)
    plt.legend()
    plt.show()


def main():
    system_len = int(input("Ingrese el tamaño del sistema operativo: "))
    num_procesos = int(input("Ingrese el número de procesos: "))

    procesos = []
    for i in range(num_procesos):
        tamano_proceso = int(input(f"Ingrese el tamaño del proceso {i + 1}: "))
        tiempo_ejecucion = int(input(f"Ingrese el tiempo de ejecución del proceso {i + 1}: "))
        procesos.append((tamano_proceso, tiempo_ejecucion))


if __name__ == "__main__":
    main()