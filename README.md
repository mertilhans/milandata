 ## MiniSHELL
# Process Nedir ?
- Process Çalışan bir programın örneği,kendi bellek alanı,dosya tanımlayıcıları ve durum bilgisi ile.
- İşlem oluşturma fork() : Bir parent process kendisinin bir kopyası olan bir child process oluşturması fork() un dönüş değeri parent ta child PIDS'Sİ child'da 0 ve bunun önemi .
- İşlem Bekleme ; wait() ve waitpid() ebeveyn sürecin çocuk süreçlerin tamamlanmasını beklemesi ve onların durum bilgilerini alması waitpid() ile belirli bir süreci bekleme veya engellemeyen non blocking bekleme
- İşlem değiştirme ; execve() Bir sürecin mevcut programını yeni bir programla değiştirmesi execve()' nin parametrelerini yürütlecek dosya yolu argümanlar ortam değişkenleri,
- İşlem iletişimi (Inter - Process COmmunication - IPC) ; pipe() iki süreç arasında tek yönlü veri akışı sağlayan basit bir IPC mekanizması. Boru hatlarının nasıl oluşturulduğu ve dosya tanımlıyıclarının kullanımı
- Sinyaller : Süreçler arası basit mesajlaşma
  # FİLE DESCRİPTORS
- stdin(0) ; Standart giriş.
- stdout(1): Standart çıkış
- stderr(2) : Standart hata.
- Dosya tanımlayıcılarının İşlevi: İşletim sisteminin açık dosyaları,soketleri ve diğer G/Ç kaynaklarını referanslamak için kullandığı tamsayı değerleri
- Yönlendirme(Redirection) : KOMUTLARIN STANDART G/Ç akışlarını dosyarla veya dier süreçlerle değiştirmesi.
- - Giriş Yönlendirme (<) Bir komutun girişini bir dosyadan okuması.
- Çıkış Yönlendirme(>, >>) Bİr komutun çıktısını bir dosyaya yazması (tek $ > $ mevcut içeriği siler çift olan mevcut içeriğe ekler)
# ORTAM DEĞİŞKENLERİ (Environment Variables)
- Tanım ve amaç: Çalışan programların ve shell'in davranışını etkileyen anahtar - değer çiftleri.
- Önemli ORtam Değişkenleri:
- PATH : Çalıştırılabilir dosyaların aranacağı dizinlerin listesi.
- HOME : Kullanıcının ana dizini
- PWD : Mevcut çalışma dizini
- OLDPWD : önceki çalışma dizini.
- USER : Kullanıcının kullanıcı adı:
- Erişim ve Yönetim C dilinde getenv setenv unsetenv extern char **environ ile
# C PROGRAMLAMA DİLİ VE SİSTEM ÇAĞRILARI 
- Bellek YÖnetimi
- - Dİnamik Bellek ayırma : malloc calloc realloc free
  - Bellek sızıntıları için free veya garbage collector
  - Dangling pointers:Serbest bırakılmış belleğe erişimden kaçınma
  - String İşlemleri
  - C STRİNG YAPISI : Null ile sonlanan karakter dizileri.
  - Standart STring FOnksiyonları
  - strlen , strcpy strncpy strncmp strndup strrchr strstr strtok
  - Hata YÖnetimi - Sistem Çağrısı Hataları: Sistem çağrılarının dönüş değerlerini kontrol etme.
  - errno Global değişkeni: SOn sistem çağrısnın neden olduğu hatayı içeren global değişken .
  - perror strerror : Hata mesajlarını kullanıcıya yazdırma.
    # TEMEL SİSTEM ÇAĞRILARI
-  fork(): Yeni işlem oluşturma. Ebeveyn ve çocuk süreçlerin PID'leri.
- execve(): Programı değiştirme. Komut yolu, argüman dizisi (char *const argv[]), ortam değişkenleri dizisi (char *const envp[]).
- wait(), waitpid(): Çocuk süreçleri bekleme. $status parametresini kullanarak çocuk sürecin çıkış durumunu alma (WIFEXITED, WEXITSTATUS vb. makrolar).
- pipe(): Süreçler arası boru hattı oluşturma. `$fd[0]$ (okuma ucu), $fd[1]$ (yazma ucu).
- $dup()$, $dup2()$: Dosya tanımlayıcılarını kopyalama ve yönlendirme. Özellikle G/Ç yönlendirmeleri ve boru hatlarında kritik.
- $open()$, $close()$, $read()$, $write()$: Dosya açma, kapama, okuma, yazma işlemleri. $flags (O_RDONLY, O_WRONLY, O_CREAT vb.), $mode (dosya izinleri) parametreleri.
- $chdir()$: Mevcut çalışma dizinini değiştirme ($cd$ built-in komutu için).
- $getcwd()$: Mevcut çalışma dizinini alma.
- $exit()$: Bir sürecin çıkışını kontrol etme.
  # KOmut Ayrıştırma (Parsing)
-Shell'in en karmaşık ve önemli kısımlarından biridir.Kullanıcının girdisini anlamlı parçalara ayırmayı içerir.
- Lexical Analysis(Lexing / Tokenization)
- Token Nedir: Girdinin en küçük anlamlı birimleri (örneğin komut adı , argüman,operatör tırnak işareti);
- TOken oluşturma Girdi dizesini karakter karakter tarayarak token'ları tanıma
- Token türleri: $command $ARGUMENT $PIPE $REDIRECTION_IN $REDIRECTION_OUT $DOUBLE_QUOTE, $SINGLE_QUOTE
- Syntax Analysis (Parsing)
- Abstract Syntax Tree - AST : Tokenları alarak komutun yapısını temsil eden bir ağaç yapısı oluşturma.Her düğüm bir komutu,boru hattını veya yönlendirmeyi temsil edebilir
- Parsing Kuralları : Shell gramerini takip ederek komutların doğru sözdizimini kontrol etme .Hatalı sözdizimi durumlarını yönetme.
- Öncelik Kuralları: Operatörlerin önceliklerini doğru bir şekilde işleme.
# Shell Operatörleri 
- PIPE - bir komutun çıktısını diğerinin girdisine bağlama.
- < GİRİŞ YÖNLENDİRE : Komutun girdisini bir dosyadan okuma.
- HERE DOCUMENT HEREDOC << giriş için belirlenen bir sınırlayıcıya kadar olan satırları okuma
- Çıkıl YÖnlendirme > komutun çıktısını bir dosyaya yazma (overwrite)
- Çıkış ekleme yönlendirme >> - Komutun çıktısını bir dosyaya ekleme (append).
- Çıkış Ekleme Yölendirme >> Komutun çıktısını bir dosyaya ekleme (append).
- Tırnak İşaretleri ve Kaçış Karakterleri
- Tek Tırnak ' İçindeki her şeyi kelimenin tam anlamıyla yorumlar.değişken genişletme veya diğer özel karakter işleme yapmaz.
- Çift tırnak "  İçindeki çoğu karakteri kelimenin tam anlamıyla yorumlar, ancak değişken genişletmeye ve ters eğik çizgi kaçış karakterline izin verir.
- Kaçış Karakteri \ Özel anlamı olan karakterleri literal olarak yorumlamak için kullanılır
- Genişletmeler (Expansions)
- ORtam Değişkeni Genişletme $VAR BİÇİMİNDEKİ DEĞİŞKEN REFERANSLARINI GERÇEK DEĞERLERİYLE DEĞİŞTİRME
- Tilde Genişletme Kullanıcının ana diziniyle değiştirme
  # Komut Yürütme
- Built'in KOmutlar
- cd Dİzin değiştirme chdir() kullanır.
- echo : Argümanları standart çıktıya yazma.Genişletmeleri (tırnaklar,değişkenler) doğru işlemesi gerekir.
- pwd . Mevcut çalışma dizinin yazdırma getcwd() kullanır.
- export: ORtam değişkeni ekleme veya değiştirme . setenv veya manuel environ manipülasyonu.
- unset - ORtam değişkeni silme unsetenv() veya manuel environ manioülasyonu
- env : TÜm ortam değişkenlerini yazdırma. environ dizisini kullanır.
- exit : Shell'den çıkma.Çıkış kodu yönetimi.
- PIPE DEV
- Boru Hattı Oluşturma pipe()
- Dosya Tanımlayıcısı Yönlendirmesi
-- ilk komut çıktısını boru hattının yazma ucuna yönlendirme.
  - Son komutun girdisini boru hattının okuma ucundan yönlendirme
  - Ara komutların girdisini önceki boru hattının okuma ucundani çıktısını sonraki boru hattının yazma ucuna yönlendirme
  - Boru Hattı Uçlarını kapatma
-Giriş/Çıkış Yönlendirmelerinin Yürütülmesi
- Dosya Açma open() ile ilgili dosyayı açma
- Dosya Tanımlayıcı Yönlendirmesi dup2 ile standart G/Ç yi açılan dosyanın dosya tanımlayıcısına yönlendirme.
- Dosya Kapatma : Yönlendirme tamamlandıktan sonra orijinal dosya tanımlayıcısını kapatma.
  # SİNYAL HANDLİNG
-SIGINT (CTRL + C ) Mevcut çalışan komutu durdurma ve yeni bir shell istemi gösterme .Shell'in kendisinin kapanmasını önleme.
-SIGQUIT (Ctrl+) Genellikle shell' de yoksayılır veya varsayılan davranışı uygulanır core dump Minishell'de yoksaymak daha güvenli olabilir.
-EQF (CTRL + D ) girişin sonunu işaretler BOş bir satırda basıldığında shell'i kapatma (exit komutu gibi)
-Sinyal Yakalama signal() veya sigaction fonksiyonlarını kullanarak sinyalleri yakalama ve özel işleyiciler atama.
