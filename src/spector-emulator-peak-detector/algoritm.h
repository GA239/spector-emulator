#ifndef ALGORITM_H
#define ALGORITM_H

#include <math.h>

/* Эта функция вычисляет среднее (m) и стандартное отклонение используя оценку: sum((xi -m)^2)/N) */
void average_std_calculation(double NumberElements, /* -NumberElements: Количество элементов во входном векторе, на которое рассчитывается среднее и стандартное отклонение */
                             double * DataIn,       /* -DataIn: Указатель на вектор входных данных длиной NumberElements */
                             double * AverageFilter,/* -AverageFilter: Указатель на вектор, в котором будет сохранено среднее значение */
                             double * StdDevFilter, /* -StdDevFilter: Указатель на вектор, в котором будет сохранено стандартное значение */
                             int Sample,            /* -Sample: Индекс последнего образца данных, который будет использован для вычисления среднего и стандартного */
                             int SampleStore){      /* -SampleStore: Индекс образца данных, в котором среднее и стандартное будут сохранены*/
    /* Описание внутренних переменных */
    int idx           = 0;
    double accumulator   = 0;
    double currentMean   = 0;
    double currentSample = 0;

    /* Среднее вычисление */
    for(idx=0;idx<NumberElements;idx++){
        accumulator+=DataIn[Sample-idx];
    }
    currentMean=accumulator/NumberElements;
    AverageFilter[SampleStore] = currentMean;

    accumulator=0;
    idx=0;

    /*Вычисление стандартного отклонения */
    for(idx=0;idx<NumberElements;idx++){
        currentSample = DataIn[Sample-idx];
        accumulator += (currentSample-currentMean)*(currentSample-currentMean);
    }
    StdDevFilter[SampleStore] = sqrt(accumulator/NumberElements);
}

/* Эта функция реализует алоитм обнаружения максимума слаженной нулевой отметки */
void peak_detection(double NumberElements,  /* -NumberElements: Количество элементов во входном векторе, на которое рассчитывается среднее и стандартное отклонение */
                    double * DataIn,        /* -DataIn: Указатель на вектор входных данных длины NumberElements */
                    double Lag,             /* -Lag: Число выборок, которые будут использоваться для расчета среднего и стандартного отклонения */
                    double Threshold,       /* -Threshold: Количество раз, когда стандартное отклонение, которое образец должен превышать среднее, чтобы считаться пиком */
                    double Influence,       /* -Influence: Значение от 0 до 1, которое эскалирует текущий образец, принадлежащий пику при вычислении среднего значения и std */
                    double * Signal,        /* -Signal: Выходной вектор, в котором хранится 0, не содержит пика в этом образце и 1 или -1 в противном случае */
                    double * AvgFilter,     /* -AvgFilter: Выходной вектор, сохраняющий среднее значение, вычисленное через итерации алгоритма */
                    double * StdFilter,     /* -StdDevFilter: Выходной вектор, сохраняющий стандартное отклонение, вычисленное через итерации алгоритма */
                    double * Peak,          /* -Peak: Выходной вектор, сохраняющий 1 в положениях, где максимум внутри пика был найден, 0 в противном случае */
                    double * FilteredData){ /* -FilteredData: Вектор отфильтрованных данных в зависимости от влияния  входного параметра  */
    /* Описание внутренних переменных*/
    int currentPeakIdx      = 0; /* Пример индекса текущего максимума обнаруженного пика */
    double currentPeakValue = 0; /* Значение максимума в текущем пике */
    int idx                 = 0; /* Индекс для прохождения образцов ввода данных */

    /*Инициализация начальных данных*/
    FilteredData[0]=DataIn[0];
    average_std_calculation(Lag,DataIn,AvgFilter,StdFilter,Lag-1,0);

    /* Перемещение по остальным входным данным */
    for(idx=1;idx<NumberElements;idx++){
        /*Если новое значение DataIn[idx] – указанное количество отклонений от среднего ()писуществует) */
        if ( (DataIn[idx]-AvgFilter[idx-1] > (Threshold*StdFilter[idx-1])) || (DataIn[idx]-AvgFilter[idx-1] < (-Threshold*StdFilter[idx-1])) ){
            if (DataIn[idx] > AvgFilter[idx-1]){
                /* Положительный сигнал */
                Signal[idx] = 1;
                    if (DataIn[idx] >= currentPeakValue){  /* сравнить с более высоким значением, чем текущий пик */
                        Peak[currentPeakIdx] = 0;
                        Peak[idx] = 1;
                        currentPeakValue = DataIn[idx];
                        currentPeakIdx = idx;
                    }
            }
            else{
                /* Отрицательный сигнал */
                Signal[idx] = -1;

                    if (DataIn[idx] <= currentPeakValue){
                        Peak[currentPeakIdx] = 0;
                        Peak[idx] = 1;
                        currentPeakValue = DataIn[idx];
                        currentPeakIdx = idx;
                    }

            }
            /* Снизить влияние текущего образца, фильтровать входные данные */
            FilteredData[idx] = Influence*DataIn[idx]+(1-Influence)*FilteredData[idx-1];
        }
        else{
            /* Отсутствие пика */
            currentPeakValue = 0;
            currentPeakIdx = idx;
            Signal[idx] = 0;
            FilteredData[idx] = DataIn[idx];
        }
        /* Обновление фильтров */
        if(idx-Lag>0){
            average_std_calculation(Lag,FilteredData,AvgFilter,StdFilter,idx,idx);

        }
        else{
            AvgFilter[idx] = AvgFilter[0];
            StdFilter[idx] = StdFilter[0];
        }
    }
}
#endif // ALGORITM_H
