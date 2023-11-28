import matplotlib.pyplot as plt
import collections as col

class Node:
    def __init__(self, base, free):
        self.base = base
        self.free = free
        self.next = None

class Process:
    last_id = 0  

    def __init__(self, size, time):
        Process.last_id += 1  
        self.id = Process.last_id
        self.size = size
        self.time = time

class LinkedList:
    # Initialize the head of the linked list, this list will manage the free memory
    def __init__(self, ram):
        self.head = Node(0, ram)

    def print_list(self):
        current = self.head
        while current:
            print(f"Base: {current.base} | Free: {current.free}")
            current = current.next
        print()

    # Make a new node, set its pointer to the head, and set the head to the new node
    def push(self, base):
        if self.head.free - base < 0:
            print("No hay suficiente espacio libre para agregar este proceso.")
            return 0
        new_node = Node(base + self.head.base, self.head.free - base)
        new_node.next = self.head.next
        self.head = new_node
        return 1
        
    def delete_node(self, target_base):
        current = self.head

        # If the target is the head, update the head to the next node
        if current and current.base == target_base:
            self.head = current.next
            return

        # Search for the target node
        prev = None
        while current and current.base != target_base:
            prev = current
            current = current.next

        # If the target node is found, remove it from the list
        if current:
            prev.next = current.next

    # sorting methods
    def sort_ascending(self):
        if self.head is None:
            return

        swapped = True
        while swapped:
            swapped = False
            node = self.head
            while node.next:
                if node.free > node.next.free:
                    node.free, node.next.free = node.next.free, node.free #swap
                    swapped = True
                node = node.next

    def sort_descending(self):
        if self.head is None:
            return

        swapped = True
        while swapped:
            swapped = False
            node = self.head
            while node.next:
                if node.free < node.next.free:
                    node.free, node.next.free = node.next.free, node.free #swap
                    swapped = True
                node = node.next

    # first fit allocation
    def first_fit_allocation(self, process_list, processes_exec, processes_waiting, ram, system_len, memory):
        event_counter = 0 # number of events
        for process in process_list:
            if self.head.free >= process.size: # if there is enough space
                self.push(process.size)
                processes_exec.append(process) # add to execution list
                event_counter += 1
                make_bar(ram, system_len, processes_exec, memory, method='Primer ajuste') # plot
            else:
                processes_waiting.append(process) # add to waiting list

        while processes_exec:
            for process in processes_exec: # for each process in execution list
                process.time -= 1 # decrease time
                print(f"Proceso {process.id} | Tiempo restante: {process.time}") 
                if process.time == 0: # if time is over
                    processes_exec.remove(process) # remove from execution list
                    print(f"Proceso {process.id} terminado.")
                    self.delete_node(process.size)
                    make_bar(ram, system_len, processes_exec, memory, method='Primer ajuste')
                    event_counter += 1
        print(f"Eventos: {event_counter}")


    # utils
    # def merge_free_spaces(self):
    #     if self.head is None:
    #         return

    #     node = self.head
    #     while node.next:
    #         if node.free and node.next.free:
    #             node.free -= node.next.free
    #             node.next = node.next.next
    #         else:
    #             node = node.next     
    
def make_bar(ram, os_len, processes_exec, mem, method='Primer ajuste'):
    fig, ax = plt.subplots(1, 1, figsize=(5, 7.5), sharey=True)

    ax.set_ylim(0, ram)
    ax.set_xticks([0])
    ax.set_xticklabels(['RAM'])
    ax.set_title(method)
    ax.bar(0, os_len, width=0.5, bottom=0, align='center', label='OS')
    bottom = os_len
    for i, process in enumerate(processes_exec):
        ax.bar(0, process.size, width=0.5, bottom=bottom, align='center', label=f'Proceso {process.id}')
        bottom += process.size
    ax.bar(0, mem.head.free, width=0.5, bottom=(mem.head.base), align='center', label=f'Espacio libre', color='grey')
    ax.legend()

    plt.show()


def main():
    ram = 1000 #int(input("Ingrese el tamaño de la memoria RAM: "))
    system_len = 100 #int(input("Ingrese el tamaño del sistema operativo: "))
    # num_procesos = int(input("Ingrese el número de procesos: "))
    # for i in range(num_procesos):
    #     tamano_proceso = int(input(f"Ingrese el tamaño del proceso {i + 1}: "))
    #     tiempo_ejecucion = int(input(f"Ingrese el tiempo de ejecución del proceso {i + 1}: "))
    #     procesos.append(Process(tamano_proceso, tiempo_ejecucion)

    procesos = []
    procesos_exec = []
    procesos_waiting = []

    procesos.append(Process(150, 5))
    procesos.append(Process(800, 12))
    procesos.append(Process(60, 20))

    memory = LinkedList(ram) #RAM
    memory.push(system_len) #OS

    memory.first_fit_allocation(procesos, procesos_exec, procesos_waiting, ram, system_len, memory)

    # make_bar(ram, system_len, procesos_exec, memory)

if __name__ == "__main__":
    main()
