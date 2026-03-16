import time
import random
import matplotlib.pyplot as plt

def bubble_sort(arr):
    n = len(arr)
    for i in range(n - 1):
        for j in range(n - 1 - i):
            if arr[j] > arr[j + 1]:
                arr[j], arr[j + 1] = arr[j + 1], arr[j]
    return arr

def merge_sort(arr):
    if len(arr) <= 1:
        return arr
    mid = len(arr) // 2
    left = merge_sort(arr[:mid])
    right = merge_sort(arr[mid:])
    return merge(left, right)

def merge(left, right):
    result = []
    i = j = 0
    while i < len(left) and j < len(right):
        if left[i] < right[j]:
            result.append(left[i])
            i += 1
        else:
            result.append(right[j])
            j += 1
    result.extend(left[i:])
    result.extend(right[j:])
    return result

def quick_sort(arr):
    if len(arr) <= 1:
        return arr
    pivot = arr[-1]
    less = [x for x in arr[:-1] if x <= pivot]
    greater = [x for x in arr[:-1] if x > pivot]
    return quick_sort(less) + [pivot] + quick_sort(greater)

def measure_time(sort_func, arr):
    arr_copy = arr.copy()
    start = time.perf_counter()
    if sort_func.__name__ == "merge_sort" or sort_func.__name__ == "quick_sort":
        sorted_arr = sort_func(arr_copy)
    else:
        sorted_arr = sort_func(arr_copy)
    end = time.perf_counter()
    return end - start, sorted_arr

sizes = [10, 50, 100, 200, 500, 1000, 2000, 5000, 10000]
results = {
    "bubble": [],
    "merge": [],
    "quick": []
}

for size in sizes:
    arr = [random.randint(0, 10000) for i in range(size)]
    
    if size <= 2000:
        t, _ = measure_time(bubble_sort, arr)
        results["bubble"].append(t)
    else:
        results["bubble"].append(None)
    
    t, _ = measure_time(merge_sort, arr)
    results["merge"].append(t)
    
    t, _ = measure_time(quick_sort, arr)
    results["quick"].append(t)

print("\nРезультаты замеров времени (в секундах):")
print("Размер\tПузырьком\tСлиянием\tБыстрая")
for i, size in enumerate(sizes):
    bubble = f"{results['bubble'][i]:.6f}" if results['bubble'][i] is not None else "—"
    print(f"{size}\t{bubble}\t{results['merge'][i]:.6f}\t{results['quick'][i]:.6f}")

plt.figure(figsize=(10, 6))
plt.plot(sizes[:len(results['bubble'])], results['bubble'], 'o-', label='Пузырьковая')
plt.plot(sizes, results['merge'], 's-', label='Слиянием')
plt.plot(sizes, results['quick'], '^-', label='Быстрая')
plt.xlabel('Количество элементов в массиве')
plt.ylabel('Время выполнения, с')
plt.title('Сравнение времени выполнения алгоритмов сортировки')
plt.legend()
plt.grid(True)
plt.savefig('sorting_comparison.png')
plt.show()