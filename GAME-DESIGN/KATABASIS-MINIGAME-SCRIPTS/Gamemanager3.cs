using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class Gamemanager3 : MonoBehaviour
{
    public GameObject enemy;
    public GameObject effect;
    public Light player;
    public GameObject resultsUI;
    public TextMeshProUGUI timerText;
    public TextMeshProUGUI timerText2;
    public static Gamemanager3 instance;
    public float HP = 30.0f;
    public float boostVal = 30.0f;
    public bool boost = false;
    public bool pause = false;
    private int yRange = 24;
    private int xRange = 42;
    private int spawnRate = 1;
    private int time = 60;

    Coroutine spawnRoutine;
    Coroutine rateRoutine;
    Coroutine timeRoutine;

    private void Awake()
    {
        instance = this;
    }

    // Start is called before the first frame update
    void Start()
    {
        microgame3Start(true);
    }

    // Update is called once per frame
    void Update()
    {
        if(HP < 0) { 
            HP = 0;
            GameOver(false);
        }

        if (boost)
        {
            boostVal -= .05f;
        }
        else
        {
            boostVal += .025f;
        }

        if(boostVal < 0) {
            boostVal = 0;
            boost = false;
        }else if(boostVal > 30)
        {
            boostVal = 30;
        }

        player.color = new Color(1, HP/255, 0, 1);
    }

    void microgame3Start(bool firstTime)
    {
        spawnRate = 1;
        spawnRoutine = StartCoroutine(enemySpawn());
        rateRoutine = StartCoroutine(spawnRateIncrease());
        if (firstTime)
        {
            StartCoroutine(zoomEffect());
        }
        timeRoutine = StartCoroutine(timer());
    }

    IEnumerator enemySpawn()
    {
        yield return new WaitForSeconds(1.5f);
        for (int i = 0; i < spawnRate; i++)
        {
            if (Random.Range(0, 2) == 1)
            {
                Instantiate(enemy, new Vector3((2 * Random.Range(0, 2) - 1) * xRange, Random.Range(-yRange, yRange), 12.47f), enemy.transform.rotation);
            }
            else
            {
                Instantiate(enemy, new Vector3(Random.Range(-xRange, xRange), (2 * Random.Range(0, 2) - 1) * yRange, 12.47f), enemy.transform.rotation);
            }
        }
        spawnRoutine = StartCoroutine(enemySpawn());
    }

    IEnumerator spawnRateIncrease()
    {
        yield return new WaitForSeconds(16f);
        if(spawnRate < 3)
        {
            spawnRate++;
        }
        rateRoutine = StartCoroutine(spawnRateIncrease());
    }

    IEnumerator zoomEffect()
    {
        yield return new WaitForSeconds(1f);
        Instantiate(effect, effect.transform.position, effect.transform.rotation);
        StartCoroutine(zoomEffect());
    }

    IEnumerator timer()
    {
        yield return new WaitForSeconds(1f);
        time--;
        timerText.text = "" + time;
        timerText2.text = "" + time;
        if(time != 0)
        {
            timeRoutine = StartCoroutine(timer());
        }
        else
        {
            GameOver(true);
        }
    }

    void GameOver(bool good)
    {
        pause = true;
        player.gameObject.SetActive(good);
        resultsUI.SetActive(true);
        resultsUI.transform.GetChild(0).gameObject.SetActive(!good);
        resultsUI.transform.GetChild(1).gameObject.SetActive(!good);
        resultsUI.transform.GetChild(2).gameObject.SetActive(good);
        resultsUI.transform.GetChild(3).gameObject.SetActive(good);
        resultsUI.transform.GetChild(4).gameObject.SetActive(good);
        StopCoroutine(spawnRoutine);
        StopCoroutine(rateRoutine);
        StopCoroutine(timeRoutine);
    }


    public void restart()
    {
        player.gameObject.SetActive(true);
        pause = false;
        HP = 30.0f;
        boostVal = 30.0f;
        boost = false;
        spawnRate = 1;
        time = 60;
        timerText.text = "60";
        timerText2.text = "60";
        player.gameObject.transform.position = new Vector3(0, 12.3f, 12.47f);
        resultsUI.SetActive(false);
        microgame3Start(false);
    }

    public void moveOn()
    {

    }
}
