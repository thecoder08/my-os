int strlen(char* string) {
    int length = 0;
    for (int i = 0; string[i] != 0; i++) {
        length++;
    }
    return length;
}
int strcmp(char* string1, char* string2) {
    if (strlen(string1) == strlen(string2)) {
        for (int i = 0; string1[i] != 0; i++) {
            if (string1[i] != string2[i]) {
                return 0;
            }
        }
        return 1;
    }
    else {
        return 0;
    }
}
void strcpy(char* source, char* dest) {
    for (int i = 0; source[i] != 0; i++) {
        dest[i] = source[i];
    }
}
char* strcat(char* string1, char* string2) {
    static char buffer[100];
    strcpy(string1, buffer);
    strcpy(string2, buffer + strlen(string1));
    buffer[strlen(string1) + strlen(string2)] = 0;
    return buffer;
}
// itoa implementation from geeksforgeeks

/* A utility function to reverse a string  */
void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        char temp = *(str+start);
        *(str+start) = *(str+end);
        *(str+end) = temp;
        start++;
        end--;
    }
}
 
// Implementation of itoa()
char* itoa(int num, int base)
{
    static char str[100];
    int i = 0;
    int isNegative = 0;
 
    /* Handle 0 explicitly, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = 1;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }
 
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse(str, i);
 
    return str;
}