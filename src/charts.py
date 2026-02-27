#!/usr/bin/env python3
import matplotlib.pyplot as plt
import re
import sys
import subprocess
import glob

def run_benchmarks(bench_program, test_files_pattern):
    """Запускает бенчмарк для всех файлов и собирает результаты"""
    data = []
    
    files = sorted(glob.glob(test_files_pattern))
    
    if not files:
        print(f"No files found matching pattern: {test_files_pattern}")
        return data
    
    print(f"Found {len(files)} test files")
    
    for file in files:
        print(f"Running benchmark on {file}...")
        try:
            # Открываем файл и передаем его содержимое через stdin
            with open(file, 'r') as f:
                result = subprocess.run([bench_program], 
                                      stdin=f,
                                      capture_output=True, 
                                      text=True, 
                                      timeout=60)
            
            output = result.stdout
            
            # Парсим вывод
            lines_match = re.search(r'Count of lines is (\d+)', output)
            counting_match = re.search(r'Counting sort time: (\d+)us', output)
            stl_match = re.search(r'STL stable sort time: (\d+)us', output)
            
            if lines_match and counting_match and stl_match:
                n = int(lines_match.group(1))
                counting = int(counting_match.group(1))
                stl = int(stl_match.group(1))
                data.append((n, counting, stl))
                print(f"  n={n}, counting={counting}us, stl={stl}us")
            else:
                print(f"  Failed to parse output")
        
        except subprocess.TimeoutExpired:
            print(f"  Timeout on {file}")
        except Exception as e:
            print(f"  Error on {file}: {e}")
    
    # Сортируем по количеству элементов
    data.sort(key=lambda x: x[0])
    
    return data

def plot_graphs(data):
    """Строит графики"""
    if not data:
        print("No data to plot!")
        return
    
    n_values = [d[0] for d in data]
    counting_times = [d[1] for d in data]
    stl_times = [d[2] for d in data]
    
    # Создаем фигуру с двумя графиками
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))
    
    # График 1: Сравнение времени выполнения
    ax1.plot(n_values, counting_times, marker='o', label='Counting Sort', linewidth=2, markersize=6)
    ax1.plot(n_values, stl_times, marker='s', label='STL stable_sort', linewidth=2, markersize=6)
    ax1.set_xlabel('Количество элементов', fontsize=12)
    ax1.set_ylabel('Время (мкс)', fontsize=12)
    ax1.set_title('Сравнение времени выполнения', fontsize=14, fontweight='bold')
    ax1.legend(fontsize=11)
    ax1.grid(True, alpha=0.3)
    ax1.set_xscale('log')
    ax1.set_yscale('log')
    
    # График 2: Ускорение (speedup)
    speedup = [stl / counting if counting > 0 else 0 for stl, counting in zip(stl_times, counting_times)]
    ax2.plot(n_values, speedup, marker='D', color='green', linewidth=2, markersize=6)
    ax2.axhline(y=1, color='r', linestyle='--', label='Нет ускорения', alpha=0.7)
    ax2.set_xlabel('Количество элементов', fontsize=12)
    ax2.set_ylabel('Ускорение (x раз)', fontsize=12)
    ax2.set_title('Ускорение Counting Sort относительно STL', fontsize=14, fontweight='bold')
    ax2.legend(fontsize=11)
    ax2.grid(True, alpha=0.3)
    ax2.set_xscale('log')
    
    plt.tight_layout()
    plt.savefig('benchmark_results.png', dpi=300, bbox_inches='tight')
    print("\nГрафик сохранен в benchmark_results.png")
    
    # Печатаем таблицу результатов
    print("\n" + "="*70)
    print(f"{'N':<12} {'Counting Sort':<18} {'STL Sort':<18} {'Speedup':<10}")
    print("="*70)
    for i, n in enumerate(n_values):
        print(f"{n:<12} {counting_times[i]:<18} {stl_times[i]:<18} {speedup[i]:<10.2f}x")
    print("="*70)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 plot_bench.py <bench_program> <test_files_pattern>")
        print("Example: python3 plot_bench.py ./bench 'cases/*.txt'")
        sys.exit(1)
    
    bench_program = sys.argv[1]
    test_files_pattern = sys.argv[2]
    
    print("Running benchmarks...")
    data = run_benchmarks(bench_program, test_files_pattern)
    
    if data:
        plot_graphs(data)
    else:
        print("No data collected!")
