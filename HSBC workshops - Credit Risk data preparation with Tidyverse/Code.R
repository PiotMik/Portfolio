library(tidyverse)

#Set theme globally and center titles
theme_set(theme_bw())
theme_update(plot.title = element_text(hjust=0.5))

setwd('C:/Users/admin/Desktop/Classes/Mgr/Semestr_1/Warsztaty/HSBC - Credit Risk data preparation with Tidyverse/')
DATA <- read_csv('KNMF_data.csv')


## Sex -> plec

DATA %>% 
  select(Sex) %>%
  group_by(Sex) %>%
  summarize(n=n()) %>%
  mutate(percent = n/sum(n))

# Convert variable type from Sex <character> to Sex <factor>
DATA <- DATA %>% mutate(Sex = as_factor(Sex))

# Visualize
theme_minimal()
DATA %>% 
  ggplot(aes(x=Sex))+
  geom_bar()

library(scales)
DATA %>% 
  select(Sex) %>%
  group_by(Sex) %>%
  summarize(n=n()) %>%
  mutate(percent = n/sum(n))%>%
  ggplot(aes(x=Sex, y=percent))+
  geom_bar(stat='identity')+
  scale_y_continuous(name = "Percent", breaks = seq(-1,1,0.1),
                     labels = percent_format(accuracy=1))+
  ggtitle('Distribution of loaners with respect to sex')+
  theme(plot.title= element_text(hjust= 0.5))


## Housing -> stan zamieszkania
library(janitor)

# counting manually and with janitor
DATA <- DATA %>% mutate(Housing = as_factor(Housing))
DATA %>% tabyl(Housing) # tabyl is a func from library janitor

# Make a barplot of housing status 
DATA %>% 
  tabyl(Housing) %>%
  ggplot(aes(x=Housing, y=percent))+
  geom_bar(stat='identity')+
  scale_y_continuous(name = "Percent", breaks = seq(-1,1,0.1),
                     labels = percent_format(accuracy=1))+
  ggtitle('Distribution of housing status')

## Purpose -> cel wziecia pozyczki

DATA %>% tabyl(Purpose)
# Zauwazmy ze niektore zmienne sa super rzadkie - bedzie problem przy modelu.
# Patrz np co sie stanie jak 3 osoby wezma kredyt na wakacje i 3 nie zdefaultuja.
# Model bedzie przewidywal ze kredyty na wakajki sa super, bo zawsze wracaja.

# Zbijmy wszystkie które stanowia < 10% w jedna grupe.
DATA <- DATA %>% mutate(Purpose = fct_lump(Purpose, prop=0.1))
DATA %>% tabyl(Purpose) # here we have made an 'Other' category
# Innym rozwiazaniem czesto spotykanym jest zbicie tematyczne

DATA <- DATA %>% mutate(Purpose = as_factor(Purpose))


## Savings Account -> wysokosc oszczednosci na koncie oszczednosciowym
DATA %>% tabyl(Saving.accounts)
DATA <- DATA %>% mutate(Saving.accounts = as_factor(Saving.accounts))
# Niestety cala masa N/A. Co z nimi? Usunac 20% obserwacji - lipa.
# Propozycja to uzupelnienie za pomoca 'dominanty' - moda.

# R nie liczy mody, oto ona:
mode <- function(data){
  num_un <- unique(data)
  mod <- num_un[which.max(tabulate(match(data, num_un)))]
}

# Uzupelniamy mode tam gdzie mamy N/A
DATA <- DATA %>% mutate(Saving.accounts = if_else(is.na(Saving.accounts), mode(Saving.accounts), Saving.accounts))

# Jaka jest róznica miedzy rich i quite rich? Lol, zadna, zrobmy 1 grupe.
DATA <- DATA %>% mutate(Saving.accounts = fct_recode(Saving.accounts, "quite rich" = "rich"))


## Checking account
DATA %>% tabyl(Checking.account)
# Kurde, az 40% N/A. Jak to uzupelnic?
# Z drugiej strony czy sie rozni bardzo od savings account? Nope.
# Usunmy ja zatem.
DATA <- DATA %>% mutate(Checking.account = as_factor(Checking.account))
DATA <- DATA %>% select(-Checking.account)


## Risk -> to nasze defaulty
DATA %>% tabyl(Risk)
DATA<- DATA %>% mutate(Risk = as_factor(Risk))

## Age -> wiek
summary(DATA$Age)
# Za NA wrzucmy mediane
DATA <-  DATA %>% mutate(Age = if_else(is.na(Age), median(Age, na.rm = TRUE), Age))
# Jak to wyglada? 
DATA %>% ggplot(aes(x= Age))+geom_histogram(binwidth = 2*IQR(DATA$Age)/length(DATA$Age)^(1/3))
# Ten dziwny binwidth to dodany zgodnie z zasada 'Jakiegos-Diakonisa'

## Job 
DATA %>% tabyl(Job)

DATA <- DATA %>% mutate(Job = as_factor(Job))
DATA <- DATA %>% mutate(Job = fct_recode(Job,
                                         "Unskilled" = "1",
                                         "Unskilled" = "2",
                                         "Skilled" = "3",
                                         "Hskilled" = "4"))

## Credit.amount
# 1 EUR = 1.95583 DEM
DATA <- DATA %>% mutate(credit.amount.eur=credit.amount/1.95583) %>% select(-credit.amount)

## Duration
DATA %>% tabyl(Duration)

## Other_credit
DATA %>% tabyl(Other_credit)

## emp.length
DATA %>% tabyl(emp.length)

## month.inc
summary(DATA$month.inc.eur)
DATA <- DATA %>% mutate(month.inc.eur = month.inc/1.95583) %>% select(-month.inc)

# O, a co to? Ujemna obserwacja? Outlier!
DATA %>% ggplot(aes(x=month.inc.eur))+geom_histogram(binwidth=10)
DATA <- DATA %>% filter(month.inc.eur > 0)

# DTI -> Debt to income ratio
summary(DATA$DTI)

# installment -> splata
# Installmenty sa N/A niektore. To akurat mozna ladnie policzyc z danych (wysokosc kredytu/dlugosc splaty) 
DATA <- DATA %>% 
  mutate(installment = if_else(is.na(installment), median(installment, na.rm=T), installment))

summary(DATA$installment)

## Zostaly jakies N/A ?
DATA %>% summarise_all(~sum(is.na(.)))


### KONIEC OGLADANIA DANYCH

# Jak zmienne miedzy soba pracuja?
library(ggcorrplot)

# Macierz scatterplotow dla zmiennych ilosciowych
DATA %>%
  select_if(is.numeric) %>%
  pairs()

# Lipa, bo nieliniowe. Moze Corr. Spearmana?
# !!!!! Super wizualizacja !!!!!
DATA %>% 
  select_if(is.numeric) %>% 
  cor(method="spearman")%>%
  ggcorrplot(type='lower', lab=T)

# Sa bardzo duze korelacje (DTI vs month.inc.eur)
DATA <- DATA %>% select(-month.inc.eur)

library(DescTools)

# A dla zmiennych jakosciowych? Cramer Von Mises!
DATA %>% select_if(is.factor) %>%
  PairApply(CramerV) %>%
  ggcorrplot(type='lower', lab=T)

### Analiza skupien.
# czyli klastrowanie
# Zrobimy metoda k-srednich. Ile klastrow dobrac? Ciezkie pytanie, sprawa expercka.
# My wezmiemy 3.


# WAZNE! Standaryzacja zmiennych
DATA_s <- DATA
DATA_s <- DATA %>%
  select(Age, DTI, emp.length) %>%
  mutate(Age_s = scale(Age),
         DTI_s = scale(DTI),
         emp.length_s = scale(emp.length)) %>%
  select(-c(Age, DTI, emp.length))

# Algorytm k-srednich
clust <- DATA_s %>% kmeans(centers= 3)
clust_tidy <- as_tibble(clust$centers)

clust_tidy <- clust_tidy %>%
  mutate(Age = Age_s * sd(DATA$Age) + mean(DATA$Age),
         DTI = DTI_s * sd(DATA$DTI) + mean(DATA$DTI),
         emp.length = emp.length_s * sd(DATA$emp.length) + mean(DATA$emp.length)) %>%
  select(-c(Age, DTI, emp.length))

library(plotly)

# Graficznie w 3D !! <3 !!
DATA %>%
  plot_ly(x=~Age, y=~DTI, z=~emp.length, color = clust$cluster) %>%
  add_markers(size=1.5)


### MODEL FITTING:
#  Train-test split
DATA= DATA %>% add_column(id = 1:nrow(DATA))
train = DATA %>% sample_frac(.7)
test = anti_join(DATA, train, by='id')


# Regresja logistyczna:
model <- glm(Risk ~., family = 'binomial', data =subset(train, select=c(-id)))
summary(model)

# Predykcje na test set
pred <- predict(model, newdata = test, type = "response")
library(pROC)

# Krzywa ROC
roc_curve  = roc(response = test$Risk, predictor =  pred)
plot(roc_curve)
roc_curve$auc # O kurde jakie AUC