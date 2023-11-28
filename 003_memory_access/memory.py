import matplotlib.pyplot as plt
import collections as col
import copy

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
        self.dir = None

class LinkedList:
    # Initialize the head of the linked list, this list will manage the free memory
    def __init__(self, ram):
        self.head = Node(0, ram)

    def print_list(self):
        print("Nodos con espacio libre:")
        current = self.head
        while current:
            print(f"Base: {current.base} | Free: {current.free}")
            current = current.next
        print()

    # Make a new node, set its pointer to the head, and set the head to the new node
    def push(self, size):
        if self.head.free >= size: # Only add the node if there is enough free space
            self.head.base += size
            self.head.free -= size
        
    # Make a new node, set its pointer to the head, and set it as the new head
    def push_free(self, base, size):
        new_node = Node(base, size)
        new_node.next = self.head
        self.head = new_node

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
    def sort_by_base(self):
        if self.head is None:
            return

        swapped = True 
        while swapped:
            swapped = False
            current = self.head
            while current.next is not None:
                if current.base > current.next.base: 
                    current.base, current.next.base = current.next.base, current.base #swap
                    current.free, current.next.free = current.next.free, current.free #swap
                    swapped = True
                current = current.next

    # merge free spaces
    def merge(self, method):
        self.sort_by_base()
        current = self.head 
        while current is not None: # for each node
            runner = current
            while runner is not None and runner.next is not None: # each node
                if current.base + current.free == runner.next.base: # if next to
                    current.free += runner.next.free # merge
                    runner.next = runner.next.next # delete
                else:
                    runner = runner.next
            current = current.next

        # eliminated code cos it were best to order in the allocation methods :p
        
        # if method == 'best':
        #     self.sort_ascending()
        # elif method == 'worst':
        #     self.sort_descending()
        # self.print_list()

    # first fit allocation
    def first_fit_allocation(self, process_list, ram, system_len, memory):
        processes_exec = []
        processes_waiting = []
        event_counter = 0 
        process_list_copy = copy.deepcopy(process_list) 
        for process in process_list_copy:
            current = self.head
            while current: # Iterate over all nodes
                if current.free >= process.size: # if there is enough space
                    current.free -= process.size
                    process.dir = current.base
                    current.base += process.size
                    processes_exec.append(process) # add to execution list
                    event_counter += 1
                    memory.print_list()
                    make_bar(ram, system_len, processes_exec, memory, method='Primer ajuste') # plot
                    break
                current = current.next
            else: # If no break occurred in the loop
                processes_waiting.append(process) # add to waiting list

        while processes_exec:
            finished_processes = [process for process in processes_exec if process.time == 0] # List of finished processes
            for process in finished_processes: # for each finished process
                processes_exec.remove(process) # remove from execution list
                print(f"----------\nProceso {process.id} terminado.\n----------")
                # Add a new node to represent the free space
                self.push_free(process.dir, process.size)
                self.merge(method='first')
                event_counter += 1
                memory.print_list()
                make_bar(ram, system_len, processes_exec, memory, method='Primer ajuste')

            for process in processes_exec: # for each process in execution list
                process.time -= 1 # decrease time
                print(f"Proceso {process.id} | Tiempo restante: {process.time}") 

            # Check if any waiting processes can be added
            for waiting_process in processes_waiting[:]: # Iterate over a slice copy of the list
                current = self.head
                while current: # Iterate over all nodes
                    if current.free >= waiting_process.size: # if there is enough space
                        current.free -= waiting_process.size
                        waiting_process.dir = current.base
                        current.base += waiting_process.size
                        processes_exec.append(waiting_process) # add to execution list
                        processes_waiting.remove(waiting_process) # remove from waiting list
                        event_counter += 1
                        memory.print_list()
                        make_bar(ram, system_len, processes_exec, memory, method='Primer ajuste')
                        break
                    current = current.next
        print(f"-------\nEventos: {event_counter}")
        return event_counter
    
    # best fit allocation
    def best_fit_allocation(self, process_list, ram, system_len, memory):
        processes_exec = []
        processes_waiting = []
        event_counter = 0 # number of events
        process_list_copy = copy.deepcopy(process_list) # make a copy of the process list
        for process in process_list_copy:
            memory.print_list()
            current = self.head
            best_fit = None
            min_size = float('inf')
            while current: # Iterate over all nodes
                if current.free >= process.size and current.free < min_size: # if there is enough space and it's smaller than the current best fit
                    min_size = current.free
                    best_fit = current
                current = current.next
            if best_fit is not None: # if a best fit was found
                best_fit.free -= process.size
                process.dir = best_fit.base
                best_fit.base += process.size
                processes_exec.append(process) # add to execution list
                event_counter += 1
                memory.print_list()
                make_bar(ram, system_len, processes_exec, memory, method='Mejor ajuste') # plot
            else: # If no best fit was found
                processes_waiting.append(process) # add to waiting list
        
        while processes_exec:
            finished_processes = [process for process in processes_exec if process.time == 0] # List of finished processes
            for process in finished_processes: # for each finished process
                processes_exec.remove(process) # remove from execution list
                print(f"----------\nProceso {process.id} terminado.\n----------")
                # Add a new node to represent the free space
                self.push_free(process.dir, process.size)
                self.merge(method='best')
                event_counter += 1
                memory.print_list()
                make_bar(ram, system_len, processes_exec, memory, method='Mejor ajuste')

            for process in processes_exec: # for each process in execution list
                process.time -= 1 # decrease time
                print(f"Proceso {process.id} | Tiempo restante: {process.time}") 

            # Check if any waiting processes can be added
            for waiting_process in processes_waiting[:]: # Iterate over a slice copy of the list
                current = self.head
                best_fit = None
                min_difference = float('inf')
                while current: # Iterate over all nodes
                    if current.free >= waiting_process.size and (current.free - waiting_process.size) < min_difference: # if there is enough space and it's smaller than the current best fit
                        min_difference = current.free - waiting_process.size
                        best_fit = current
                    current = current.next
                if best_fit is not None: # if a best fit was found
                    best_fit.free -= waiting_process.size
                    waiting_process.dir = best_fit.base
                    best_fit.base += waiting_process.size
                    processes_exec.append(waiting_process) # add to execution list
                    processes_waiting.remove(waiting_process) # remove from waiting list
                    event_counter += 1
                    memory.print_list()
                    make_bar(ram, system_len, processes_exec, memory, method='Mejor ajuste')
        print(f"-------\nEventos: {event_counter}")
        return event_counter

    # worst fit allocation
    def worst_fit_allocation(self, process_list, ram, system_len, memory):
        processes_exec = []
        processes_waiting = []
        event_counter = 0 
        process_list_copy = copy.deepcopy(process_list)  # make a copy of the process list
        for process in process_list_copy: # for each process
            current = self.head # start at the head
            worst_fit = None # initialize the worst fit node
            max_size = 0 # initialize the max size
            while current: # Iterate over all nodes
                if current.free >= process.size and current.free > max_size: # if there is enough space and it's bigger than the current worst fit
                    worst_fit = current # update worst fit
                    max_size = current.free # update max size
                current = current.next # move to the next node

            if worst_fit is not None: # if a worst fit was found
                worst_fit.free -= process.size # allocate the process
                process.dir = worst_fit.base  # set the base 
                worst_fit.base += process.size # update the base
                processes_exec.append(process) # add to execution list
                event_counter += 1 
                memory.print_list()
                make_bar(ram, system_len, processes_exec, memory, method='Worst Fit')
            else:
                processes_waiting.append(process)

        while processes_exec:
            finished_processes = [process for process in processes_exec if process.time == 0] # List of finished processes
            for process in finished_processes: # for each finished process
                processes_exec.remove(process) # remove from execution list
                print(f"----------\nProceso {process.id} terminado.\n----------")
                # Add a new node to represent the free space
                self.push_free(process.dir, process.size)
                self.merge(method='worst')
                event_counter += 1
                make_bar(ram, system_len, processes_exec, memory, method='Peor ajuste')

            for process in processes_exec: # for each process in execution list
                process.time -= 1 # decrease time
                # print(f"Proceso {process.id} | Tiempo restante: {process.time}") 

            # Check if any waiting processes can be added
            for waiting_process in processes_waiting[:]: # Iterate over a slice copy of the list
                current = self.head 
                worst_fit = None
                max_size = -1
                while current: # Iterate over all nodes
                    if current.free >= waiting_process.size and current.free > max_size: # if there is enough space and it's larger than the current worst fit
                        max_size = current.free
                        worst_fit = current
                    current = current.next
                if worst_fit is not None: # if a worst fit was found
                    worst_fit.free -= waiting_process.size
                    waiting_process.dir = worst_fit.base
                    worst_fit.base += waiting_process.size
                    processes_exec.append(waiting_process) # add to execution list
                    processes_waiting.remove(waiting_process) # remove from waiting list
                    event_counter += 1
                    memory.print_list()
                    make_bar(ram, system_len, processes_exec, memory, method='Peor ajuste')
        print(f"-------\nEventos: {event_counter}")
        return event_counter

def make_bar(ram, os_len, processes_exec, mem, method):
    fig, ax = plt.subplots(1, 1, figsize=(5, 7.5), sharey=True)

    ax.set_ylim(0, ram)
    ax.set_xticks([0])
    ax.set_xticklabels(['RAM'])
    ax.set_title(method)
    ax.bar(0, os_len, width=0.5, bottom=0, align='center', label='OS')
    bottom = os_len
    print(f"-------\nOS | Base: 0 | Tamaño: {os_len}\n-------\nProcesos:")
    for i, process in enumerate(processes_exec): # for each process
        if process.dir is None:
            process.dir = bottom # set base

        ax.bar(0, process.size, width=0.5, bottom=process.dir, align='center', label=f'Proceso {process.id}')
        bottom += process.size # update bottom
        print(f"Proceso {process.id} | Base: {process.dir} | Tamaño: {process.size}")
    current = mem.head # free memory
    while current is not None and current.free > 0: # for each node
        ax.bar(0, current.free, width=0.5, bottom=current.base, align='center', label=f'Espacio libre', color='grey') 
        current = current.next
    ax.legend()

    plt.show()

def plot_events(first_fit, best_fit, worst_fit):
    methods = ['Primer ajuste', 'Mejor ajuste', 'Peor ajuste']
    events = [first_fit, best_fit, worst_fit]

    plt.bar(methods, events)
    plt.xlabel('Método de asignación')
    plt.ylabel('Número de eventos')
    plt.title('Comparación de métodos de asignación')
    plt.show()

def main():
    procesos = []

    # DATA
    ram = 2000
    system_len = 400

    procesos.append(Process(200, 5))
    procesos.append(Process(800, 10))
    procesos.append(Process(400, 5))
    procesos.append(Process(100, 12))
    procesos.append(Process(150, 15))
    procesos.append(Process(60, 3))
    procesos.append(Process(300, 10))
    procesos.append(Process(100, 5))

    # ram = int(input("Ingrese el tamaño de la memoria RAM: "))
    # system_len = int(input("Ingrese el tamaño del sistema operativo: "))
    # num_procesos = int(input("Ingrese el número de procesos: "))
    # for i in range(num_procesos):
    #     tamano_proceso = int(input(f"Ingrese el tamaño del proceso {i + 1}: "))
    #     tiempo_ejecucion = int(input(f"Ingrese el tiempo de ejecución del proceso {i + 1}: "))
    #     procesos.append(Process(tamano_proceso, tiempo_ejecucion))

    memory = LinkedList(ram) #RAM
    memory.push(system_len) #OS

    first_fit = memory.first_fit_allocation(procesos, ram, system_len, memory)
    best_fit = memory.best_fit_allocation(procesos, ram, system_len, memory)
    worst_fit = memory.worst_fit_allocation(procesos, ram, system_len, memory)

    print(f"Primer ajuste: {first_fit} eventos")
    print(f"Mejor ajuste: {best_fit} eventos")
    print(f"Peor ajuste: {worst_fit} eventos")

    plot_events(first_fit, best_fit, worst_fit)

if __name__ == "__main__":
    main()
