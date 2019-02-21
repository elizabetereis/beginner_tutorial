# beginner_tutorial - Suggestions

* rever Workspace Organization:
  * não existe mais hyro_template, essentials
* rever Creating your first package
  * json: faltou os ¨{}¨ do inicio e fim do json
* internal classes:
  * faltou incluir test_internal_classes.cpp no cmakelists
  * trocar j8 por j
  * corrigir nome da file que vai os testes
* primeiro componente:
  * o nome do logger no construtor está incorreto.
  * não usar a macro assertandrun.
  * add métodos de assert separados
  * como é que testa o connect na versão nova???
  * tem que mandar o cara rodar o teste e ver a saída.
  * adicionar construtor com (Uri) no .h
  * mudar o a variavel para m_signal_generator
  * atualizar o connect no teste
  * TEST (ExampleTest, SignalGeneratorComponentCheck) mudar primeiro parametro;
* segundo component:
  * `std::shared_ptr<hyro::HyroLogger> DigitalConverterComponent::s_loggerhyro::HyroLoggerManager::CreateLogger("DigitalConverterComponent `
  * faltar completar o código;
  * static std::shared_ptr<hyro::HyroLogger> s_logger
  * incluir o ";";
  * porque os gets e sets são privates?
  * TEST (DigitalConverterTest, DigitalConverterCheck) mudar nome
