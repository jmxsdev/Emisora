## 📁 **Archivos de Entrada para Testing**

### **Set 1: Datos Básicos (prueba simple)**

**canciones1.in**
```
5
Bohemian Rhapsody 5 55 95
Imagine 3 04 90
Yesterday 2 05 85  
Billie Jean 4 54 80
Sweet Child O Mine 5 56 75
```

**publicidad1.in**
```
Coca Cola 30 5
Pepsi 25 3
Nike 20 4
```

**shows1.in**
```
3
Morning News 30 00 3 8
Sports Update 15 00 2 7
Music Hour 60 00 1 6
```

### **Set 2: Datos Medianos (prueba intermedia)**

**canciones2.in**
```
8
Hotel California 6 30 92
Stairway to Heaven 8 02 96
Smells Like Teen Spirit 5 01 88
Like a Rolling Stone 6 13 91
Sweet Home Alabama 4 45 84
Purple Haze 2 50 79
Johnny B. Goode 2 40 86
Thriller 5 57 93
```

**publicidad2.in**
```
Microsoft 45 8
Apple 30 6
Google 25 10
Samsung 35 7
Amazon 40 5
```

**shows2.in**
```
5
Business Today 45 00 2 9
Tech Talk 30 00 3 8
Health Corner 20 00 4 7
Entertainment News 25 00 2 6
Culture Hour 60 00 1 5
```

### **Set 3: Datos Complejos (prueba exhaustiva)**

**canciones3.in**
```
12
Bohemian Rhapsody 5 55 95
Imagine 3 04 90
Yesterday 2 05 85
Billie Jean 4 54 80
Sweet Child O Mine 5 56 75
Hotel California 6 30 92
Stairway to Heaven 8 02 96
Smells Like Teen Spirit 5 01 88
Like a Rolling Stone 6 13 91
Sweet Home Alabama 4 45 84
Purple Haze 2 50 79
Johnny B. Goode 2 40 86
```

**publicidad3.in**
```
Microsoft Windows 45 12
Apple iPhone 30 8
Google Search 25 15
Samsung Galaxy 35 10
Amazon Prime 40 6
Tesla Motors 50 4
Netflix 20 7
Spotify 15 9
```

**shows3.in**
```
7
Morning News 30 00 3 9
Sports Central 45 00 2 8
Tech Today 60 00 1 7
Health Watch 20 00 4 6
Entertainment Tonight 25 00 3 5
Culture Talk 40 00 2 4
Business Report 35 00 2 3
```

## 🧪 **Cómo Testear el Programa**

### **1. Preparación de Archivos**
```bash
# Crear directorio de prueba
mkdir test_radio
cd test_radio

# Copiar el programa C (guardar como radio_program.c)
# Crear los archivos de entrada para cada test
```

### **2. Compilación del Programa**
```bash
# Compilar el programa
gcc -o radio_program radio_program.c -Wall -Wextra

# Verificar que no hay warnings
```

### **3. Ejecución de Tests**

#### **Test 1: Datos Básicos**
```bash
# Copiar archivos del set 1
cp canciones1.in canciones.in
cp publicidad1.in publicidad.in  
cp shows1.in shows.in

# Ejecutar programa
./radio_program

# En el menú seleccionar:
# 1 → Generar grilla para toda la semana
# Verificar que se crean 7 archivos grilla_*.out
```

#### **Test 2: Datos Medianos**
```bash
# Cambiar a set 2
cp canciones2.in canciones.in
cp publicidad2.in publicidad.in
cp shows2.in shows.in

# Ejecutar y probar consultas
./radio_program
# Seleccionar opción 2 para consultar momentos específicos
```

#### **Test 3: Datos Complejos**  
```bash
# Cambiar a set 3
cp canciones3.in canciones.in
cp publicidad3.in publicidad.in
cp shows3.in shows.in

# Probar todas las funcionalidades
./radio_program
```

### **4. Verificación de Resultados**

#### **Chequear Archivos de Salida**
```bash
# Ver contenido de las grillas generadas
head -n 10 grilla_lunes.out
wc -l grilla_*.out  # Contar líneas por archivo

# Verificar formato
cat grilla_lunes.out | grep "00 05 00"  # Primera entrada
```

#### **Validar Reglas del Negocio**
```bash
# Verificar que no se repiten canciones antes de 4 horas
cat grilla_lunes.out | grep "C " | sort  # Ver canciones

# Verificar publicidades (deben cumplirse las cuotas)
cat grilla_lunes.out | grep "P " | wc -l  # Total publicidades
cat grilla_lunes.out | grep "P Coca" | wc -l  # Cuota específica

# Verificar horarios estelares (deben tener shows)
cat grilla_lunes.out | grep "07:" | grep "S "
cat grilla_lunes.out | grep "12:" | grep "S "  
cat grilla_lunes.out | grep "18:" | grep "S "
```

### **5. Testing Automático (Script Bash)**

**test_radio.sh**
```bash
#!/bin/bash

echo "=== TESTING RADIO PROGRAM ==="

# Test 1: Datos básicos
echo "Test 1: Datos básicos"
cp test_files/canciones1.in canciones.in
cp test_files/publicidad1.in publicidad.in
cp test_files/shows1.in shows.in

./radio_program << EOF
1
4
EOF

echo "Archivos generados:"
ls -la grilla_*.out
echo ""

# Test 2: Datos medianos  
echo "Test 2: Datos medianos"
cp test_files/canciones2.in canciones.in
cp test_files/publicidad2.in publicidad.in
cp test_files/shows2.in shows.in

./radio_program << EOF
1
4
EOF

echo "Test completado"
```

### **6. Pruebas Específicas a Realizar**

#### **Prueba de Horarios Estelares**
```bash
# Verificar que entre 7-9, 12-14, 18-19 hay shows
grep -n "07:" grilla_lunes.out | head -5
grep -n "12:" grilla_lunes.out | head -5  
grep -n "18:" grilla_lunes.out | head -5
```

#### **Prueba de Publicidades**
```bash
# Verificar que se cumplen todas las cuotas
for empresa in "Coca" "Pepsi" "Nike"; do
    count=$(grep "P $empresa" grilla_lunes.out | wc -l)
    echo "$empresa: $count cuñas"
done
```

#### **Prueba de Formato**
```bash
# Verificar formato HH MM SS T Nombre
awk '{print NF}' grilla_lunes.out | sort -u  # Debe ser 5 campos
```

#### **Prueba de Tiempo Total**
```bash
# Calcular tiempo total programado
last_line=$(tail -1 grilla_lunes.out)
last_time=$(echo $last_line | awk '{print $1*3600 + $2*60 + $3}')
echo "Tiempo total programado: $last_time segundos"
```

### **7. Errores Comunes a Verificar**

1. **Memory leaks**: Aunque usamos memoria estática
2. **Formato incorrecto** en archivos de salida
3. **Cuotas de publicidad** no cumplidas
4. **Canciones repetidas** antes de 4 horas
5. **Shows repetidos** el mismo día
6. **Tiempos solapados** entre elementos

### **8. Métricas de Calidad**

```bash
# Tiempo de ejecución
time ./radio_program <<< "1"

# Uso de memoria (aunque es estático)
size radio_program

# Archivos generados
du -h grilla_*.out
```

## 📋 **Checklist de Testing**

- [ ] Programa compila sin warnings
- [ ] Lee correctamente los 3 archivos de entrada
- [ ] Genera 7 archivos de salida
- [ ] Formato de salida correcto (HH MM SS T Nombre)
- [ ] Respeta separación de 1 segundo
- [ ] Cumple cuotas de publicidad
- [ ] No repite canciones antes de 4 horas
- [ ] No repite shows el mismo día
- [ ] Prioriza shows en horarios estelares
- [ ] Programa desde 00:05:00 hasta 23:59:59
- [ ] Consultas por momento específico funcionan
- [ ] Cada día tiene programación diferente