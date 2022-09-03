# Посторенние автокорреляционной функции (АКФ)

Программа, реализованная с применением GUI, позволяющая строить АКФ для трех стандартных для радиотехники сигналов:

- прямоугольный импульсов
- треугольный импульсов
- |sinc|

Для построения необходимо сначала выбрать сигнал, кликнув мышью на соответствующую кнопку, после этого станет возможным нажать кнопку "старт". 
	
Так как основной целью данной программы является иллюстрация процесса построения автокорреляционной функции, АКФ (нижний график) строится, динамически, отображая изменения коэффициента корреляции в зависимости от временного сдвига сигнала относительно самого себя. Также процесс сдвигания копии сигнала отображен на графике сигнала (верхний график). При необходимости можно приостановить построение, возобновить или начать сначала. При возникновении вопросов можно получить краткую теоретическую справку и сведения о работе программы, кликнув на кнопку со знаком вопроса.
	
Проект разработан в интегрируемой среде разработки **Qt Creator** на языке программирования **C++**. Интерфейс реализован в кроссплатформенной среде разработки графических интерфейсов **Qt Designer**. Графики в проекте реализованы с помощью библиотеки для построения графиков **QtChart**. За основу цветового оформления программы была взята тема из библиотеки QChart "ChartThemeBlueIcy". Для динамического построения использовались возможности таймера библиотеки **QTimer**. 
	
  
*Программа создана в рамках сотрудничества с кафедрой 410 МАИ (НИУ) и успешно внедрена в учебный процесс. Что подтверждается соответствующим документом, копия которого приложена в репозитории проекта.*
 
*В репозиторие проекта представлено видео с демонстрацией функционала программы.*
