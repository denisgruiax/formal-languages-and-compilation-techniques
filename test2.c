void func();

void main()
{
    int a, b, c;

    printf("a = ");
    scanf("%i", &a);

    printf("b = ");
    scanf("%i", &b);

    printf("c = ");
    scanf("%i", &c);
    func(a, b, c);
}

void func(int a, int b, int c)
{
    if ((a != b) && (a != c) && (b != c))
    {
        printf("%i si %i\n", a, b);
        printf("%i si %i\n", a, c);
        printf("%i si %i\n", b, c);
    }
    else
    {
        if (a != b)
        {
            printf("%i si %i\n", a, b);
        }
        else if (a != c)
        {
            printf("%i si %i\n", a, b);
        }
        else
        {
            printf("%i si %i\n", b, c);
        }
    }
}