# Crear directorio de prueba
mkdir test_radio
cd test_radio

# Copiar el programa C (guardar como radio_program.c)
# Crear los archivos de entrada para cada test

# Compilar el programa
gcc -o radio_program radio_program.c -Wall -Wextra

# Verificar que no hay warnings

# Copiar archivos del set 1
cp canciones1.in canciones.in
cp publicidad1.in publicidad.in  
cp shows1.in shows.in

# Ejecutar programa
./radio_program

# En el menú seleccionar:
# 1 → Generar grilla para toda la semana
# Verificar que se crean 7 archivos grilla_*.out

# Cambiar a set 2
cp canciones2.in canciones.in
cp publicidad2.in publicidad.in
cp shows2.in shows.in

# Ejecutar y probar consultas
./radio_program
# Seleccionar opción 2 para consultar momentos específicos

# Cambiar a set 3
cp canciones3.in canciones.in
cp publicidad3.in publicidad.in
cp shows3.in shows.in

# Probar todas las funcionalidades
./radio_program

# Ver contenido de las grillas generadas
head -n 10 grilla_lunes.out
wc -l grilla_*.out  # Contar líneas por archivo

# Verificar formato
cat grilla_lunes.out | grep "00 05 00"  # Primera entrada

# Verificar que no se repiten canciones antes de 4 horas
cat grilla_lunes.out | grep "C " | sort  # Ver canciones

# Verificar publicidades (deben cumplirse las cuotas)
cat grilla_lunes.out | grep "P " | wc -l  # Total publicidades
cat grilla_lunes.out | grep "P Coca" | wc -l  # Cuota específica

# Verificar horarios estelares (deben tener shows)
cat grilla_lunes.out | grep "07:" | grep "S "
cat grilla_lunes.out | grep "12:" | grep "S "  
cat grilla_lunes.out | grep "18:" | grep "S "

# Verificar que entre 7-9, 12-14, 18-19 hay shows
grep -n "07:" grilla_lunes.out | head -5
grep -n "12:" grilla_lunes.out | head -5  
grep -n "18:" grilla_lunes.out | head -5

# Verificar que se cumplen todas las cuotas
for empresa in "Coca" "Pepsi" "Nike"; do
    count=$(grep "P $empresa" grilla_lunes.out | wc -l)
    echo "$empresa: $count cuñas"
done

# Verificar formato HH MM SS T Nombre
awk '{print NF}' grilla_lunes.out | sort -u  # Debe ser 5 campos

# Calcular tiempo total programado
last_line=$(tail -1 grilla_lunes.out)
last_time=$(echo $last_line | awk '{print $1*3600 + $2*60 + $3}')
echo "Tiempo total programado: $last_time segundos"