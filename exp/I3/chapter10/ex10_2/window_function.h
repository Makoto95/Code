void Hanning_window(double w[], int N)
{
  int n;

  if (N % 2 == 0) /* N�������̂Ƃ� */
  {
    for (n = 0; n < N; n++)
    {
      w[n] = 0.5 - 0.5 * cos(2.0 * M_PI * n / N);
    }
  }
  else /* N����̂Ƃ� */
  {
    for (n = 0; n < N; n++)
    {
      w[n] = 0.5 - 0.5 * cos(2.0 * M_PI * (n + 0.5) / N);
    }
  }
}
