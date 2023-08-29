#include <main.h>

void setup()
{
  // запускаем UART
  Serial.begin(115200);

  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // Разрешаем TRACE
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;            // Разрешаем счетчик тактов

  // init ION
  pinMode(ION_Ground_PIN, OUTPUT);
  digitalWrite(ION_Ground_PIN, LOW);
  pinMode(ION_ADC_PIN, INPUT_ANALOG);
  pinMode(ION_Power_PIN, OUTPUT);
  digitalWrite(ION_Power_PIN, HIGH);
}

uint16_t Filtr_val(uint16_t *old_val, uint16_t new_val)
{
  return (Filtr_A * *old_val + Filtr_B * new_val) << Filtr_K;
}
long Filtr_val_uint64(uint64_t *old_val, uint64_t new_val)
{
  return (Filtr_A * *old_val + Filtr_B * new_val) << Filtr_K;
}

void loop()
{
  char buff[512];
  if (Benchmarks_int < Benchmarks_multiplier)
  {
    Serial.print("\nBenchmark STARTED. #");
    Serial.println(Benchmarks_int + 1);

    Serial.println("Test 1. ROM.");
    StartTime;
    uint8_t big_mass[big_mass_size];
    for (uint64_t i = 0; i < const_mass_multiplier; i++)
    {
      for (uint64_t j = 0; j < const_mass_size; j++)
      {
        big_mass[i * const_mass_size + j] = (uint8_t)const_mass[j];
      }
    }
    PrintTime(1);

    Serial.println("Test 2. RAM.");
    StartTime;
    for (uint64_t i = 0; i < big_mass_size; i++)
    {
      big_mass[i] = big_mass[i] * 13;
    }
    uint8_t big_mass2[big_mass_size];
    for (uint64_t i = 0; i < big_mass_size; i++)
    {
      big_mass2[i] = big_mass[big_mass_size - 1 - i];
    }
    for (uint64_t i = 0; i < big_mass_size; i++)
    {
      big_mass[i] = big_mass[i] - big_mass2[i];
    }
    PrintTime(2);

    Serial.println("Test 3. ALU.");
    StartTime;
    for (uint64_t i = 0; i < big_mass_size; i++)
    {
      big_mass[i] = (uint8_t)(ceil(cos((float)big_mass[i] * 1.3 / 7.0) * 10.9));
    }
    PrintTime(3);

    uint16_t adc_val;
    Serial.println("Test 4. ADC speed.");
    StartTime;
    for (uint64_t i = 0; i < ADC_INT_speed; i++)
    {
      adc_val += analogRead(ION_ADC_PIN);
    }
    PrintTime(4);

    Serial.println("Test 5. ADC jitter and readind ION.");
    StartTime;
    adc_val = analogRead(ION_ADC_PIN);
    uint16_t min_val = adc_val;
    uint16_t max_val = adc_val;
    uint16_t med_val = adc_val;
    uint16_t min_med = med_val;
    uint16_t max_med = med_val;
    for (uint64_t i = 0; i < ADC_INT_jitter; i++)
    {
      adc_val = analogRead(ION_ADC_PIN);
      Filtr_val(&med_val, adc_val);
      if (adc_val < med_val)
      {
        Filtr_val(&min_med, adc_val);
      }
      else if (adc_val > med_val)
      {
        Filtr_val(&max_med, adc_val);
      }
      if (adc_val < min_val)
      {
        min_val = adc_val;
      }
      else if (adc_val > max_val)
      {
        max_val = adc_val;
      }
    }
    PrintTime(5);
    BResult[Benchmarks_int][4][2] = (uint64_t)med_val;
    sprintf(buff, "min_val=%d, min_med=%d, med_val=%d, max_med=%d, max_val=%d", min_val, min_med, med_val, max_med, max_val);
    Serial.println(buff);

    Serial.println("Test 6. GPIO speed.");
    StartTime;
    for (uint64_t i = 0; i < GPIO_INT_speed; i++)
    {
      pinMode(GPIO_SPEED_PIN, OUTPUT);
      digitalWrite(GPIO_SPEED_PIN, i % 2);
      pinMode(GPIO_SPEED_PIN, INPUT);
      digitalWrite(GPIO_SPEED_PIN, (i + 1) % 2);
    }
    PrintTime(6);

    Serial.println("\nResults:");
    Serial.println("ROM\tRAM\tALU\tADC_speed\tADC_jitter\tGPIO_speed");
    sprintf(buff, "%d\t%d\t%d\t%d\t%d\t%d", test1_time, test2_time, test3_time, test4_time, test5_time, test6_time);
    Serial.println(buff);

    Serial.println("\nBenchmark END.");
    Benchmarks_int++;
  }
  else
  {
    if (Benchmarks_int == Benchmarks_multiplier)
    {
      for (uint8_t i = 0; i < Benchmarks_tests; i++)
      {
        for (uint8_t j = 0; j < Benchmarks_multiplier; j++)
        {
          Filtr_val_uint64(&BResult[0][i][0], BResult[j][i][0]);
          Filtr_val_uint64(&BResult[0][i][0], BResult[j][i][1]);
        }
      }
      Serial.println("\nAverage values:");
      Serial.println("Test\tpc\tmcs");
      for (uint8_t i = 0; i < Benchmarks_tests; i++)
      {
        sprintf(buff, "%d\t%ld\t%ld\t%ld", i + 1, (long)BResult[0][i][0], (long)BResult[0][i][1], (long)BResult[0][i][2]);
        Serial.println(buff);
      }
      Benchmarks_int++;
    }
    else
    {
      // Serial.println("\nBenchmark SLEEP.");
      delay(5000);
    }
  }
}